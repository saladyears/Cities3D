/*
 *  Cities3D - Copyright (C) 2001-2009 Jason Fugate (saladyears@gmail.com)
 * 
 *  This program is free software; you can redistribute it and/or modify it 
 *  under the terms of the GNU General Public License as published by the Free 
 *  Software Foundation; either version 2 of the License, or (at your option) 
 *  any later version.
 * 
 *  This program is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
 *  for more details.
 */
#include "stdwx.h"
#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//
#include "OthersTradeDialog.h"
#include "ISizer.h"
#include "Game.h"
#include "ImageEngine.h"
#include "SelectionImageStrip.h"
#include "ResourcePoolImageStrip.h"
#include "TradeLogic.h"
#include "BaseStaticText.h"
#include "BaseStaticBox.h"
#include "FinalTradeBox.h"
#include "BaseButton.h"
#include "GameEngine.h"
#include "DataObject.h"
#include "Controller.h"
#include "ConfigDatabase.h"
#include "TimerManager.h"
#include "INetworkEngine.h"
#include "IRuleEngine.h"
#include "SkinManager.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	enum
	{
		ID_OFFER_BUTTON			= 0,
		ID_TRADE_OTHERS_BUTTON	,
		ID_REJECT_OTHERS_BUTTON	,
	};

	class OthersTradeLogic : public TradeLogic
	{
	public:
		OthersTradeLogic(boost::weak_ptr<Game> game, wxInt32 index) 
			: TradeLogic(game)
			, mPlayerIndex(index)
			, mLeftSide(0)
			, mRightSide(0)
		{}

		virtual void Initialize()
		{
			mLeftSide = mRightSide = 0;
			wxInteractiveImageStrip* pool = GetResourcePool();

			if(-1 != mPlayerIndex)
			{
				// Fill the resource pool with all of its initial values.
				const GamePtr game = GetGame();
				
				const Data::IntHash& resources = 
					game->playerGame(mPlayerIndex).read<Data::IntHash>(shResources);

				Data::IntHash::const_iterator it, itEnd = resources.end();
				for(it = resources.begin(); it != itEnd; ++it)
				{
					const HashString& res = it->first;
					wxInt32 count = it->second;

					// Set that number in the pool.
					pool->SetResourceCount(res, count);
				}

				wxInteractiveImageStrip* player = GetLeftSide();
				wxInteractiveImageStrip* others = GetRightSide();

				player->SetCounterColorFunc(
					boost::bind(&OthersTradeLogic::UpdateColor, this, _1));
				others->SetCounterColorFunc(
					boost::bind(&OthersTradeLogic::UpdateColor, this, _1));
			}
			else
			{
				pool->Clear();
			}

			EnableButton();
		}

		virtual bool AllowDrop(wxInteractiveImageStrip* target, wxCoord, 
			wxCoord, const HashString& res)
		{
			bool allow = false;

			// OK, so here's where the logic kicks in.  We allow drag and drops
			// in four directions: pool to player, player to pool, pool to 
			// others, and others to pool.  We do not allow player to others 
			// drops.
			wxInteractiveImageStrip* pool = GetResourcePool();
			wxInteractiveImageStrip* player = GetLeftSide();
			wxInteractiveImageStrip* others = GetRightSide();

			if(true == pool->InDrag())
			{
				if(target == others)
				{
					allow = AllowPoolToOthers(res);
				}
				else if(target == player)
				{
					allow = AllowPoolToPlayer(res);
				}
			}
			// They can always drag to the pool from either side.
			else if(target == pool)
			{
				allow = true;
			}

			return allow;
		}

		virtual void ExecuteDrop(wxInteractiveImageStrip* target, wxCoord x, 
			wxCoord y, const HashString& res)
		{
			// This is where we start adjusting counts.
			wxInteractiveImageStrip* pool = GetResourcePool();
			wxInteractiveImageStrip* player = GetLeftSide();
			wxInteractiveImageStrip* others = GetRightSide();

			wxInt32 targetAmount = target->GetResourceCount(res);

			if(true == pool->InDrag())
			{
				wxPoint pos(x, y);

				// From pool to others.
				if(target == others)
				{
					ExecutePoolToOthers(res, targetAmount);
				}
				// From pool to player.
				else if(target == player)
				{
					ExecutePoolToPlayer(res, targetAmount);
				}
			}
			// From others to pool.
			else if(true == others->InDrag())
			{
				wxASSERT(target == pool);
				ExecuteOthersToPool(res);
			}
			// From player to pool.
			else if(true == player->InDrag())
			{
				wxASSERT(target == pool);
				ExecutePlayerToPool(res, targetAmount);
			}

			EnableButton();
		}

		virtual void HandleLeftClick(wxInteractiveImageStrip* origin,
			const HashString& res)
		{
			wxInteractiveImageStrip* pool = GetResourcePool();
			wxInteractiveImageStrip* player = GetLeftSide();
			wxInteractiveImageStrip* others = GetRightSide();

			// Left double-click works in three directions, pool to player,
			// player to pool, and others to pool.
			if( (origin == pool) &&
				(true == AllowPoolToPlayer(res)))
			{
				wxInt32 targetAmount = player->GetResourceCount(res);
				ExecutePoolToPlayer(res, targetAmount);
			}
			else if(origin == player)
			{
				wxInt32 targetAmount = pool->GetResourceCount(res);
				ExecutePlayerToPool(res, targetAmount);
			}
			else if(origin == others)
			{
				ExecuteOthersToPool(res);
			}

			EnableButton();
		}

		virtual void HandleRightClick(wxInteractiveImageStrip* origin, 
			const HashString& res)
		{
			wxInteractiveImageStrip* pool = GetResourcePool();
			wxInteractiveImageStrip* others = GetRightSide();

			// Right double-click only works in one direction, from the pool
			// to the others.
			if( (origin == pool) &&
				(true == AllowPoolToOthers(res)))
			{
				wxInt32 targetAmount = others->GetResourceCount(res);
				ExecutePoolToOthers(res, targetAmount);
			}

			EnableButton();
		}

		void IgnoreAnyMatch(bool ignore)
		{
			mIgnoreAnyMatch = ignore;
		}

	private:
		bool AllowPoolToOthers(const HashString& res) const
		{
			bool ret = false;

			wxInteractiveImageStrip* others = GetRightSide();
			wxInteractiveImageStrip* player = GetLeftSide();

			wxInt32 othersAmount = others->GetResourceCount(res);
			wxInt32 playerAmount = player->GetResourceCount(res);

			wxInt32 matchCount = others->GetResourceCount(shAnyMatch);

			// They are only allowed to drop if there are open spots, or that 
			// resource is already in the target, and the resource hasn't been 
			// added to the other side already.  They may also add if there
			// are any AnyMatch cards still to be replaced.
			if( (-1 == playerAmount) &&
				((false == others->IsFull()) ||
				(0 < othersAmount) ||
				(-1 != matchCount)))
			{
				if(shAnyOffer == res)
				{
					// As long as there are no any match cards on this side, 
					// they can add a new any offer card.
					if(-1 == matchCount)
					{
						ret = true;
					}
				}
				else if(shAnyMatch == res)
				{
					ret = true;
				}
				else
				{
					const GamePtr& game = GetGame();

					// They can put as many resources in as the bank holds.
					const Data::IntHash& originalRes = 
						game->read<Data::IntHash>(shOriginalBankResources);
					
					Data::IntHash::const_iterator it = 
						originalRes.find(res);
					wxASSERT(originalRes.end() != it);
					
					wxInt32 origAmount = it->second;

					if(-1 == othersAmount)
					{
						othersAmount = 0;
					}

					ret = (othersAmount < origAmount);
				}
			}

			return ret;
		}

		bool AllowPoolToPlayer(const HashString& res) const
		{
			bool ret = false;

			wxInteractiveImageStrip* others = GetRightSide();
			wxInteractiveImageStrip* player = GetLeftSide();
			wxInteractiveImageStrip* pool = GetResourcePool();

			wxInt32 targetAmount = player->GetResourceCount(res);
			wxInt32 otherAmount = others->GetResourceCount(res);

			wxInt32 matchCount = player->GetResourceCount(shAnyMatch);

			// They are only allowed to drop if there are open spots, or that 
			// resource is already in the target, and the resource hasn't been 
			// added to the other side already.
			if( (-1 == otherAmount) &&
				((false == player->IsFull()) ||
				(0 < targetAmount) ||
				(-1 != matchCount)))
			{
				if(shAnyOffer == res)
				{
					// As long as there are no any match cards on this side, 
					// they can add a new any offer card.
					if(-1 == matchCount)
					{
						ret = true;
					}
				}
				else if(shAnyMatch == res)
				{
					ret = true;
				}
				else
				{
					// They are allowed to drop to the player side only if the
					// player has enough of that resource to trade with the 
					// others.
					wxInt32 amount = pool->GetResourceCount(res);

					ret = (0 < amount);
				}
			}

			return ret;
		}

		void ExecutePoolToOthers(const HashString& res, wxInt32 targetAmount)
		{
			wxInteractiveImageStrip* others = GetRightSide();

			wxInt32 matchCount = others->GetResourceCount(shAnyMatch);

			if(-1 == targetAmount)
			{
				others->Add(res, 1);
			}
			else
			{
				wxASSERT(0 < targetAmount);
				others->SetResourceCount(res, targetAmount + 1);
			}

			// They replace AnyMatch cards as they add new cards.
			if( (false == mIgnoreAnyMatch) &&
				(0 < matchCount))
			{
				if(1 == matchCount)
				{
					others->Remove(shAnyMatch);
				}
				else
				{
					others->SetResourceCount(shAnyMatch, matchCount - 1);
				}
			}


			++mRightSide;
		}

		void ExecutePoolToPlayer(const HashString& res, wxInt32 targetAmount)
		{
			wxInteractiveImageStrip* pool = GetResourcePool();
			wxInteractiveImageStrip* player = GetLeftSide();

			wxInt32 poolAmount = pool->GetResourceCount(res);

			wxInt32 matchCount = player->GetResourceCount(shAnyMatch);

			if(-1 == targetAmount)
			{
				player->Add(res, 1);
			}
			else
			{
				wxASSERT(0 < targetAmount);
				player->SetResourceCount(res, targetAmount + 1);
			}

			// Subtract from the pool.
			if(shAnyMatch != res)
			{
				pool->SetResourceCount(res, poolAmount - 1);
			}

			// They replace AnyMatch cards as they add new cards.
			if( (false == mIgnoreAnyMatch) &&
				(0 < matchCount))
			{
				if(1 == matchCount)
				{
					player->Remove(shAnyMatch);
				}
				else
				{
					player->SetResourceCount(shAnyMatch, matchCount - 1);
				}
			}

			++mLeftSide;
		}

		void ExecuteOthersToPool(const HashString& res)
		{
			wxInteractiveImageStrip* others = GetRightSide();
			wxInt32 othersAmount = others->GetResourceCount(res);
			wxASSERT(0 < othersAmount);
			others->SetResourceCount(res, othersAmount - 1);

			// If it's the last one, remove it from the list.
			if(1 == othersAmount)
			{
				others->Remove(res);
			}

			--mRightSide;
		}

		void ExecutePlayerToPool(const HashString &res, wxInt32 targetAmount)
		{
			wxInteractiveImageStrip* pool = GetResourcePool();
			wxInteractiveImageStrip* player = GetLeftSide();

			wxInt32 playerAmount = player->GetResourceCount(res);
			wxASSERT(0 < playerAmount);

			// Subtract that amount from the player and add it to the pool.
			if(0 == playerAmount - 1)
			{
				player->Remove(res);
			}
			else
			{
				player->SetResourceCount(res, playerAmount - 1);
			}

			if(shAnyMatch != res)
			{
				pool->SetResourceCount(res, targetAmount + 1);
			}

			--mLeftSide;
		}

		void EnableButton()
		{
			bool enable = (mLeftSide && mRightSide);

			// If there are AnyMatch cards on either side, they cannot offer
			// the trade, they need to fill in those cards.
			if( (true == enable) &&
				((-1 != GetLeftSide()->GetResourceCount(shAnyMatch)) ||
				(-1 != GetRightSide()->GetResourceCount(shAnyMatch))))
			{
				enable = false;
			}

			// Enable the trade button when possible.
			GetButton()->Enable(enable);
		}

		wxColour UpdateColor(const HashString& res)
		{
			wxColour color = wxColour(255, 255, 255);

			// We set the AnyMatch color to red to indicate that the player 
			// must replace those cards for the trade to be valid.
			if(shAnyMatch == res)
			{
				color = SKIN.Element(shPlayerUINegative);

			}

			return color;
		}

		bool mIgnoreAnyMatch;

		wxInt32 mPlayerIndex;

		// The number of valid items on each side.  They must be non
		// zero for the trade to go through.
		wxInt32 mLeftSide;
		wxInt32 mRightSide;
	};

	boost::scoped_ptr<OthersTradeLogic> spOthersTradeLogic;
	wxInteractiveImageStrip* spPlayerOthers;
	wxInteractiveImageStrip* spOthers;
	wxInteractiveImageStrip *spPoolOthers;

	wxFinalTradeBox* spTradeBox;
	wxImageStrip* spTradeLeft;
	wxImageStrip* spTradeRight;
	wxBaseButton* spTradeOthersButton;
	wxBaseButton* spRejectOthersButton;

	class Sizer_OthersTrade : public ISizer
	{
	public:
		Sizer_OthersTrade(wxWindow *window, boost::weak_ptr<Game> game, wxInt32 index) 
			: ISizer(window), mGame(game), mPlayerIndex(index) {}

		void CreateLayout(wxBoxSizer *sizer)
		{
			const GamePtr& game = mGame.lock();
			wxASSERT(game);

			wxInt32 current = game->read<wxInt32>(shCurrentPlayer);
			const wxString& name = game->playerGame(current).player().Name();

			bool trader = (mPlayerIndex == current);
			
			wxBoxSizer *vert1 = new wxBoxSizer(wxVERTICAL);

			spOthersTradeLogic.reset(
				new OthersTradeLogic(mGame, mPlayerIndex));

			wxString text1, text2;
			if(-1 != mPlayerIndex)
			{
				text1 = stYouOffer;
				text2 = stYouWant;
			}

			wxBaseStaticBox *box = new wxBaseStaticBox(mpWindow, 0, text1,
				wxDefaultPosition, wxDefaultSize, shDefaultText, 
				shTradeBoxFont);

			box->SetSecondLabel(text2);

			wxStaticBoxSizer *boxSizer = 
				new wxStaticBoxSizer(box, wxHORIZONTAL);
	
			// Create the trade image strips.
			spPlayerOthers = new wxSelectionImageStrip(mpWindow, 0, 4, 10, 
				spOthersTradeLogic.get());
			spPlayerOthers->SetFlags(wxImageStrip::FlagReverseOrder);

			spOthers = new wxSelectionImageStrip(mpWindow, 0, 4, 10, 
				spOthersTradeLogic.get());

			wxBaseButton *button = new wxBaseButton(mpWindow, ID_OFFER_BUTTON, 
				stOffer);

			boxSizer->Add(spPlayerOthers, 1, wxEXPAND | wxLEFT | wxTOP, 2);
			boxSizer->Add(button, 0, 
				wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT | wxBOTTOM, 4);
			boxSizer->Add(spOthers, 1, wxEXPAND | wxRIGHT | wxTOP, 2);

			spPoolOthers = new wxResourcePoolImageStrip(mpWindow, 0, mGame, 
				spOthersTradeLogic.get(), true);

			spOthersTradeLogic->SetParameters(spPlayerOthers, spOthers, spPoolOthers, 
				button);

			// See if we need to force max size.
			wxInt32 width = CONFIG.read(swConfigSectionUI, swTradeWithOthersSlash + swWidth, -1);
			wxInt32 height = CONFIG.read(swConfigSectionUI, swTradeWithOthersSlash + swHeight, -1);

			wxInt32 initial = 0;

			if( (-1 == width) ||
				(-1 == height))
			{
				initial = -1;
			}

			// Calculate offsets.
			wxSizeEvent event(wxSize(initial, initial));
			spPlayerOthers->ProcessEvent(event);
			spOthers->ProcessEvent(event);
			spPoolOthers->ProcessEvent(event);

			// Calculate the ratio needed to keep all cards the same size in
			// the window.
			wxInt32 poolX = spPoolOthers->GetMinSize().x;
			wxInt32 boxX = boxSizer->GetMinSize().x;
			
			// Determine buffers.
			wxInt32 x = ((boxX - poolX) / 2) + 11;

			wxBoxSizer *horz2 = new wxBoxSizer(wxHORIZONTAL);
			horz2->AddSpacer(x);
			horz2->Add(spPoolOthers, 1, wxEXPAND);
			horz2->AddSpacer(x);

			wxString str = trader ? stDragAndDropCardsOffer : stRespondWithStockyard;

			if(-1 == mPlayerIndex)
			{
				str = swEmpty;
			}

			wxBoxSizer *horz3 = new wxBoxSizer(wxHORIZONTAL);
			wxBaseStaticText *text = new wxBaseStaticText(mpWindow, -1,
				str, shDefaultText, shInfoFont);
			
			horz3->Add(text, 1, wxEXPAND | wxALL, 2);
			
			vert1->AddSpacer(6);
			vert1->Add(boxSizer, 10, wxEXPAND | wxLEFT | wxRIGHT, 12);
			vert1->Add(horz2, 10, wxEXPAND);
			vert1->Add(horz3, 0, wxALIGN_CENTER_HORIZONTAL);
			vert1->AddSpacer(6);

			spTradeBox = new wxFinalTradeBox(mpWindow, stYouGive, CR_SIZE);
			if(false == trader)
			{
				spTradeBox->SetFirstName(swEmpty, CR_SIZE);
			}
			boxSizer = new wxStaticBoxSizer(spTradeBox, wxHORIZONTAL);
	
			// Create the trade image strips.
			spTradeLeft = new wxImageStrip(mpWindow, -1, 4, 10);
			spTradeLeft->SetPixelSpacing(4);
			spTradeLeft->SetFlags(wxImageStrip::FlagReverseOrder |
				wxImageStrip::FlagAlignCenter);
			spTradeRight = new wxImageStrip(mpWindow, -1, 4, 10);
			spTradeRight->SetPixelSpacing(4);
			spTradeRight->SetFlags(wxImageStrip::FlagAlignCenter);
			
			spTradeLeft->ProcessEvent(event);
			spTradeRight->ProcessEvent(event);

			spTradeOthersButton = new wxBaseButton(mpWindow, ID_TRADE_OTHERS_BUTTON, 
				stTradeExclamation);
			spTradeOthersButton->Disable();

			// Add a reject button.
			boxSizer->Add(spTradeLeft, 1, wxEXPAND | wxLEFT | wxTOP, 2);
			
			wxBoxSizer* vert = new wxBoxSizer(wxVERTICAL);

			vert->AddStretchSpacer();
			vert->Add(spTradeOthersButton, 0, wxALIGN_CENTER_HORIZONTAL | 
				wxBOTTOM, 6);

			str = trader ? stClear : stNoThanks;

			spRejectOthersButton = new wxBaseButton(mpWindow, ID_REJECT_OTHERS_BUTTON, str);
			spRejectOthersButton->Disable();
			vert->Add(spRejectOthersButton, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 
				6);
			vert->AddStretchSpacer();

			boxSizer->Add(vert, 0, wxEXPAND | wxALIGN_CENTER_VERTICAL | 
				wxALL, 4);
			
			boxSizer->Add(spTradeRight, 1, wxEXPAND | wxRIGHT | wxTOP, 2);

			vert1->Add(boxSizer, 10, wxEXPAND | wxLEFT | wxRIGHT, 12);
			vert1->AddSpacer(4);

			if(true == trader)
			{
				str = stFinalizeYourTrade;
			}
			else if(-1 != mPlayerIndex)
			{
				str = wxString::Format(stIfAcceptsYourOffer.c_str(), name.c_str());
			}
			else
			{
				str = swEmpty;
			}
			
			wxBoxSizer *horz4 = new wxBoxSizer(wxHORIZONTAL);
			text = new wxBaseStaticText(mpWindow, -1, str, shDefaultText, 
				shInfoFont);
			
			horz4->Add(text, 1, wxEXPAND | wxALL, 2);
			vert1->Add(horz4, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM,
				4);

			sizer->Add(vert1, 1, wxEXPAND);
		}

	private:
		boost::weak_ptr<Game> mGame;
		wxInt32 mPlayerIndex;
	};

	// Hold the colors for this amount of time before clearing.
	const wxInt32 sPauseTime = 2000;
};

//---------------------------- PUBLIC           -----------------------------//
#ifdef __WIN32__
	//disables the 'this' : used in base member initializer list warning
	#pragma warning(disable:4355)
#endif
wxOthersTradeDialog::wxOthersTradeDialog(wxWindow *parent, 
	boost::weak_ptr<Game> game, wxInt32 index)
: wxBaseCloseableDialog(parent, ISizerPtr(new Sizer_OthersTrade(this, game, 
	index)), stTradeWithOthersCaps, swTradeWithOthers)
, mGame(game)
, mPlayerIndex(index)
, mPartnerIndex(-1)
, mTrader(false)
, mPlayerAccept(false)
, mPartnerAccept(false)
, mInTimer(false)
{
	const GamePtr& thisGame = game.lock();
	wxASSERT(thisGame);

	if(mPlayerIndex == thisGame->read<wxInt32>(shCurrentPlayer))
	{
		mTrader = true;
	}
	
	Controller::get().AddReceiver(shEventPlayerUI, 
		&wxOthersTradeDialog::OnPlayerUI, this);
	Controller::get().AddReceiver(shEventClickOffer,
		&wxOthersTradeDialog::OnClickOffer, this);
	Controller::get().AddReceiver(shEventAcceptOffer,
		&wxOthersTradeDialog::OnAcceptOffer, this);
	Controller::get().AddReceiver(shEventRejectOffer,
		&wxOthersTradeDialog::OnRejectOffer, this);
	Controller::get().AddReceiver(shEventClearOffer,
		&wxOthersTradeDialog::OnClearOffer, this);
	Controller::get().AddReceiver(shEventMatchOffer,
		&wxOthersTradeDialog::OnMatchOffer, this);
}

wxOthersTradeDialog::~wxOthersTradeDialog()
{
	// Remove any timer that might still be going.
	TIME.RemoveOneShotFunction(&wxOthersTradeDialog::OnTimer, this, 
		sPauseTime);

	spOthersTradeLogic.reset();

	Controller::get().RemoveReceiver(shEventPlayerUI, 
		&wxOthersTradeDialog::OnPlayerUI, this);
	Controller::get().RemoveReceiver(shEventClickOffer,
		&wxOthersTradeDialog::OnClickOffer, this);
	Controller::get().RemoveReceiver(shEventAcceptOffer,
		&wxOthersTradeDialog::OnAcceptOffer, this);
	Controller::get().RemoveReceiver(shEventRejectOffer,
		&wxOthersTradeDialog::OnRejectOffer, this);
	Controller::get().RemoveReceiver(shEventClearOffer,
		&wxOthersTradeDialog::OnClearOffer, this);
	Controller::get().RemoveReceiver(shEventMatchOffer,
		&wxOthersTradeDialog::OnMatchOffer, this);

	// If we are the trader, shut down everyone else when they decide they are
	// done trading.
	if(true == mTrader)
	{
		RULE.Execute(shRuleCloseTradeWindow, DataObject());
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxOthersTradeDialog, wxBaseCloseableDialog)
	EVT_BUTTON(ID_OFFER_BUTTON, wxOthersTradeDialog::OnOfferButton)
	EVT_BUTTON(ID_TRADE_OTHERS_BUTTON, wxOthersTradeDialog::OnTradeButton)
	EVT_BUTTON(ID_REJECT_OTHERS_BUTTON, wxOthersTradeDialog::OnRejectButton)
END_EVENT_TABLE()

void wxOthersTradeDialog::Close()
{
	// Only the trader can shut down the window.
	if(true == mTrader)
	{
		Controller::get().Transmit(shEventShutdownModelessUI, 0);
	}
}

void wxOthersTradeDialog::OnTimer()
{
	mInTimer = false;

	OnClearOffer();
}

void wxOthersTradeDialog::OnPlayerUI(const DataObject &)
{
	// Clean everything out.
	spPlayerOthers->Clear();
	spOthers->Clear();
	spOthersTradeLogic->Initialize();

	if(false == mInTimer)
	{
		OnClearOffer();
	}

	// If we're open when we get this event, force a redraw so that anything
	// underneath handles updates.
	Controller::get().Transmit(shEventDirty, 0);
}

void wxOthersTradeDialog::OnClickOffer(const DataObject& object)
{
	typedef std::map<HashString, wxInt32> ResourceMap;

	// Fill the trade sides.
	const ResourceMap& left = object.read<ResourceMap>();
	const ResourceMap& right = object.read<ResourceMap>(1);
	mPartnerIndex = object.read<wxInt32>(2);

	bool thisPartner = (mPlayerIndex == mPartnerIndex);

	if( (true == mTrader) ||
		(true == thisPartner))
	{
		spTradeOthersButton->Enable();
		spRejectOthersButton->Enable();
	}
	else
	{
		spTradeOthersButton->Disable();
		spRejectOthersButton->Disable();
	}

	const GamePtr& game = mGame.lock();
	wxASSERT(game);

	if(true == mTrader)
	{
		spTradeLeft->SetResourceMap(left);
		spTradeRight->SetResourceMap(right);

		// Set labels.
		ColorType color = game->playerGame(mPartnerIndex).color();

		spTradeBox->SetSecondName(stYouGet, color);
	}
	else
	{
		// Trades clicked on by the trader come in with their perspective,
		// which is flipped from the tradee perspective.
		spTradeLeft->SetResourceMap(right);
		spTradeRight->SetResourceMap(left);

		wxString firstName, secondName;
		ColorType firstColor;

		wxInt32 current = game->read<wxInt32>(shCurrentPlayer);
		
		// Set labels.
		if(true == thisPartner)
		{
			firstName = stYouGive;
			firstColor = CR_SIZE;
			secondName = stYouGet;
		}
		else
		{
			wxString name = game->playerGame(mPartnerIndex).player().Name();
			firstName = wxString::Format(stStringGives.c_str(), name.c_str());
			firstColor = game->playerGame(mPartnerIndex).color();

			name = game->playerGame(current).player().Name();
			secondName = wxString::Format(stStringGives.c_str(), name.c_str());		
		}

		ColorType secondColor = game->playerGame(current).color();
		
		spTradeBox->SetFirstName(firstName, firstColor);
		spTradeBox->SetSecondName(secondName, secondColor);
	}
	
	// In hotseat mode, the trading player automatically accepts the offer.
	if(shHotseat == game->read<HashString>(shMode))
	{
		RULE.Execute(shRuleAcceptOffer, DataObject(mPartnerIndex));
	}
}

void wxOthersTradeDialog::OnAcceptOffer(wxInt32 player)
{
	wxASSERT(-1 != player);

	const GamePtr& game = mGame.lock();
	wxASSERT(game);

	wxInt32 current = game->read<wxInt32>(shCurrentPlayer);

	if(player == current)
	{
		mPlayerAccept = true;

		wxFinalTradeBox::State state = mTrader ? 
			wxFinalTradeBox::StateAcceptLeft :
			wxFinalTradeBox::StateAcceptRight;

		spTradeBox->SetState(state);

		if(player == mPlayerIndex)
		{
			spTradeOthersButton->Disable();
		}
	}
	else if(player == mPartnerIndex)
	{
		wxFinalTradeBox::State state = mTrader ? 
			wxFinalTradeBox::StateAcceptRight :
			wxFinalTradeBox::StateAcceptLeft;

		mPartnerAccept = true;
		spTradeBox->SetState(state);
	}

	// If they've both accepted, and this is the current player's computer,
	// execute the trade.
	if(	(true == mPlayerAccept) &&
		(true == mPartnerAccept))
	{
		wxImageStrip::ResourceMap left, right;

		spTradeLeft->FillResourceMap(left);
		spTradeRight->FillResourceMap(right);

		// Turn on the timer that will hold the current state for a couple of
		// seconds.
		TIME.AddOneShotFunction(&wxOthersTradeDialog::OnTimer, this, 
			sPauseTime);

		mInTimer = true;

		// Make sure both buttons are disabled during the timer.
		spTradeOthersButton->Disable();
		spRejectOthersButton->Disable();

		if(true == mTrader)
		{
			DataObject object(left, right, mPartnerIndex);

			// If we're in network mode, execute this as a top level rule, even
			// though we're in the middle of another rule.
			if(true == NET.IsConnected())
			{
				NET.SendRule(shRuleExecuteTrade, object);
			}
			else
			{
				RULE.Execute(shRuleExecuteTrade, object);
			}
		}
	}
}

void wxOthersTradeDialog::OnRejectOffer(wxInt32 player)
{
	// If this is the current trade partner, they've rejected the trade.
	if(mPartnerIndex == player)
	{
		// Depending on which player this is, set the right or left side as
		// the reject side.
		wxFinalTradeBox::State state = mTrader ? 
			wxFinalTradeBox::StateRejectRight :
		wxFinalTradeBox::StateRejectLeft;

		spTradeBox->SetState(state);

		// Immediately disable the buttons so they can't come over and mess things up.
		spTradeOthersButton->Disable();
		spRejectOthersButton->Disable();

		TIME.AddOneShotFunction(&wxOthersTradeDialog::OnTimer, this, 
			sPauseTime);
	}
}

void wxOthersTradeDialog::OnClearOffer(wxInt32)
{
	// The trader wants to clear everything.
	mPlayerAccept = false;
	mPartnerAccept = false;
	mPartnerIndex = -1;

	// Clear stuff out.
	spTradeLeft->Clear();
	spTradeRight->Clear();

	spTradeOthersButton->Disable();
	spRejectOthersButton->Disable();

	if(false == mTrader)
	{
		spTradeBox->SetFirstName(swEmpty, CR_SIZE);
	}

	spTradeBox->SetSecondName(swEmpty, CR_SIZE);
	spTradeBox->SetState(wxFinalTradeBox::StateNeutral);
}

void wxOthersTradeDialog::OnMatchOffer(const DataObject& object)
{
	wxASSERT(2 <= object.numItems());

	typedef std::map<HashString, wxInt32> ResourceMap;

	const ResourceMap& left = object.read<ResourceMap>();
	const ResourceMap& right = object.read<ResourceMap>(1);

	// Clear everything in the trade offer.
	spPlayerOthers->Clear();
	spOthers->Clear();
	spOthersTradeLogic->Initialize();

	spPlayerOthers->Freeze();
	spOthers->Freeze();
	spPoolOthers->Freeze();

	// When we add AnyMatch cards on a match offer, we don't want to replace
	// them as we add other cards.
	spOthersTradeLogic->IgnoreAnyMatch(true);

	// Fill left side.
	ResourceMap::const_iterator it, itEnd = left.end();
	for(it = left.begin(); it != itEnd; ++it)
	{
		const HashString& res = it->first;
		wxInt32 amount = it->second;

		for(wxInt32 i = 0; i < amount; ++i)
		{
			spOthersTradeLogic->HandleLeftClick(spPoolOthers, res);
		}
	}

	// Fill right side.
	itEnd = right.end();
	for(it = right.begin(); it != itEnd; ++it)
	{
		const HashString& res = it->first;
		wxInt32 amount = it->second;

		for(wxInt32 i = 0; i < amount; ++i)
		{
			spOthersTradeLogic->HandleRightClick(spPoolOthers, res);
		}
	}

	spOthersTradeLogic->IgnoreAnyMatch(false);

	spPlayerOthers->Thaw();
	spOthers->Thaw();
	spPoolOthers->Thaw();
}

void wxOthersTradeDialog::OnOfferButton(wxCommandEvent &)
{
	wxImageStrip::ResourceMap left, right;

	// Create resource listings out of the two sides, then send 'em down.
	spPlayerOthers->FillResourceMap(left);
	spOthers->FillResourceMap(right);

	// Replace AnyOffer cards with AnyMatch cards before sending them out.
	wxImageStrip::ResourceMap::iterator it = left.find(shAnyOffer);
	if(left.end() != it)
	{
		wxInt32 amount = it->second;
		left.erase(it);
		left[shAnyMatch] = amount;
	}

	it = right.find(shAnyOffer);
	if(right.end() != it)
	{
		wxInt32 amount = it->second;
		right.erase(it);
		right[shAnyMatch] = amount;
	}

	RULE.Execute(shRuleOfferTrade, DataObject(left, right, mPlayerIndex));
}

void wxOthersTradeDialog::OnTradeButton(wxCommandEvent &)
{
	spTradeOthersButton->Disable();
	spRejectOthersButton->Disable();

	RULE.Execute(shRuleAcceptOffer, DataObject(mPlayerIndex));
}

void wxOthersTradeDialog::OnRejectButton(wxCommandEvent &)
{
	spTradeOthersButton->Disable();
	spRejectOthersButton->Disable();

	if(false == mTrader)
	{
		RULE.Execute(shRuleRejectOffer, DataObject(mPlayerIndex));
	}
	else
	{
		RULE.Execute(shRuleClearOffer, DataObject());
	}
}
