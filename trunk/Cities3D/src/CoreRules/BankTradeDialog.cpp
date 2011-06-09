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
#include "BankTradeDialog.h"
#include "ISizer.h"
#include "Game.h"
#include "ImageEngine.h"
#include "SelectionImageStrip.h"
#include "ResourcePoolImageStrip.h"
#include "TradeLogic.h"
#include "BaseStaticText.h"
#include "BaseStaticBox.h"
#include "BaseButton.h"
#include "GameEngine.h"
#include "DataObject.h"
#include "Controller.h"
#include "ConfigDatabase.h"
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
		ID_TRADE_BANK_BUTTON		= 0,
	};

	class BankTradeLogic : public TradeLogic
	{
	public:
		BankTradeLogic(boost::weak_ptr<Game> game) 
			: TradeLogic(game)
			, mLeftSide(0)
			, mRightSide(0)
		{}

		virtual void Initialize()
		{
			// Fill the resource pool with all of its initial values.
			const GamePtr thisGame = GetGame();

			// Go through the resources of the current player and find all the
			// ones they are capable of trading with the bank.
			wxInt32 current = thisGame->read<wxInt32>(shCurrentPlayer);

			wxInteractiveImageStrip* pool = GetResourcePool();
			
			pool->SetCounterColorFunc(
				boost::bind(&BankTradeLogic::UpdateResourcePool, this, _1));

			mLeftSide = 0;

			// If we're in auto-trade mode, there will already be resources
			// on the bank side.
			if(false == thisGame->read<HashString>(shAutoTradeLogic).empty())
			{
				mRightSide = GetRightSide()->GetResourceCount();
			}
			else
			{
				// The pool values are already set in auto-trade mode.
				const PlayerGame& playerGame = thisGame->playerGame(current);
				const Data::IntHash& resources = 
					playerGame.read<Data::IntHash>(shResources);
				Data::IntHash::const_iterator it, itEnd = resources.end();

				for(it = resources.begin(); it != itEnd; ++it)
				{
					pool->SetResourceCount(it->first, it->second);
				}

				mRightSide = 0;
			}

			EnableButton();
		}

		virtual bool AllowDrop(wxInteractiveImageStrip* target, wxCoord, 
			wxCoord, const HashString& res)
		{
			bool allow = false;

			// OK, so here's where the logic kicks in.  We allow drag and drops
			// in four directions: pool to player, player to pool, pool to bank, 
			// and bank to pool.  We do not allow player to bank drops.
			wxInteractiveImageStrip* pool = GetResourcePool();
			wxInteractiveImageStrip* player = GetLeftSide();
			wxInteractiveImageStrip* bank = GetRightSide();

			if(true == pool->InDrag())
			{
				if(target == bank)
				{
					allow = AllowPoolToBank(res);
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
			wxInteractiveImageStrip* bank = GetRightSide();

			wxInt32 targetAmount = target->GetResourceCount(res);

			if(true == pool->InDrag())
			{
				wxPoint pos(x, y);

				// From pool to bank.
				if(target == bank)
				{
					ExecutePoolToBank(res, targetAmount);
				}
				// From pool to player.
				else if(target == player)
				{
					ExecutePoolToPlayer(res, targetAmount);
				}
			}
			// From bank to pool.
			else if(true == bank->InDrag())
			{
				wxASSERT(target == pool);
				ExecuteBankToPool(res);
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
			wxInteractiveImageStrip* bank = GetRightSide();

			// Left double-click works in three directions, pool to player,
			// player to pool, and bank to pool.
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
			else if(origin == bank)
			{
				ExecuteBankToPool(res);
			}

			EnableButton();
		}

		virtual void HandleRightClick(wxInteractiveImageStrip* origin, 
			const HashString& res)
		{
			wxInteractiveImageStrip* pool = GetResourcePool();
			wxInteractiveImageStrip* bank = GetRightSide();

			// Right double-click only works in one direction, from the pool
			// to the bank.
			if( (origin == pool) &&
				(true == AllowPoolToBank(res)))
			{
				wxInt32 targetAmount = bank->GetResourceCount(res);
				ExecutePoolToBank(res, targetAmount);
			}

			EnableButton();
		}

	private:
		wxColour UpdateResourcePool(const HashString &res)
		{
			wxInt32 amount = GetResourcePool()->GetResourceCount(res);

			DataObject input(res, amount), output;
			RULE.Decide(shLogicIsResourceBankTradeable, input, output);

			wxColour color = SKIN.Element(shDefaultText);
			if(false == output.read<bool>())
			{
				color = SKIN.Element(shDisabledCardText);
			}

			return color;
		}

		bool AllowPoolToBank(const HashString& res) const
		{
			bool ret = false;

			wxInteractiveImageStrip* bank = GetRightSide();
			wxInteractiveImageStrip* player = GetLeftSide();

			wxInt32 targetAmount = bank->GetResourceCount(res);
			wxInt32 otherAmount = player->GetResourceCount(res);

			// They are only allowed to drop if there are open spots, or that 
			// resource is already in the target, and the resource hasn't been 
			// added to the other side already.
			if( (0 == (bank->GetFlags() & wxImageStrip::FlagDisabled)) &&
				(-1 == otherAmount) &&
				((false == bank->IsFull()) ||
				(0 < targetAmount)))
			{
				const GamePtr& game = GetGame();

				// If we're dragging from the pool, they are allowed to drop to 
				// the bank if there is enough of that bank resource left.
				const Data::IntHash& bankRes = 
					game->read<Data::IntHash>(shBankResources);
				
				Data::IntHash::const_iterator it = bankRes.find(res);
				wxASSERT(bankRes.end() != it);
				
				wxInt32 gameAmount = it->second;

				ret = (0 < gameAmount) && (targetAmount < gameAmount);
			}

			return ret;
		}

		bool AllowPoolToPlayer(const HashString& res) const
		{
			bool ret = false;

			wxInteractiveImageStrip* bank = GetRightSide();
			wxInteractiveImageStrip* player = GetLeftSide();
			wxInteractiveImageStrip* pool = GetResourcePool();

			wxInt32 targetAmount = player->GetResourceCount(res);
			wxInt32 otherAmount = bank->GetResourceCount(res);

			// They are only allowed to drop if there are open spots, or that 
			// resource is already in the target, and the resource hasn't been 
			// added to the other side already.
			if( (-1 == otherAmount) &&
				((false == player->IsFull()) ||
				(0 < targetAmount)))
			{

				// They are allowed to drop to the player side only if the
				// player has enough of that resource to trade with the bank.
				wxInt32 amount = pool->GetResourceCount(res);

				DataObject input(res, amount), output;
				RULE.Decide(shLogicIsResourceBankTradeable, input, output);

				ret = output.read<bool>();
			}

			return ret;
		}

		void ExecutePoolToBank(const HashString& res, wxInt32 targetAmount)
		{
			wxInteractiveImageStrip* bank = GetRightSide();

			if(-1 == targetAmount)
			{
				bank->Add(res, 1);
			}
			else
			{
				wxASSERT(0 < targetAmount);
				bank->SetResourceCount(res, targetAmount + 1);
			}

			++mRightSide;
		}

		void ExecutePoolToPlayer(const HashString& res, wxInt32 targetAmount)
		{
			wxInteractiveImageStrip* pool = GetResourcePool();
			wxInteractiveImageStrip* player = GetLeftSide();

			// Determine the trade ratio.
			wxInt32 poolAmount = pool->GetResourceCount(res);

			DataObject input(res, poolAmount), output;
			RULE.Decide(shLogicIsResourceBankTradeable, input, output);
			wxASSERT(2 <= output.numItems());
			wxASSERT(true == output.read<bool>());

			wxInt32 ratio = output.read<wxInt32>(1);

			if(-1 == targetAmount)
			{
				player->Add(res, ratio);
			}
			else
			{
				wxASSERT(0 < targetAmount);
				player->SetResourceCount(res, targetAmount + ratio);
			}

			// Subtract from the pool.
			pool->SetResourceCount(res, poolAmount - ratio);

			++mLeftSide;
		}

		void ExecuteBankToPool(const HashString& res)
		{
			wxInteractiveImageStrip* bank = GetRightSide();
			wxInt32 bankAmount = bank->GetResourceCount(res);
			wxASSERT(0 < bankAmount);
			bank->SetResourceCount(res, bankAmount - 1);

			// If it's the last one, remove it from the list.
			if(1 == bankAmount)
			{
				bank->Remove(res);
			}

			--mRightSide;
		}

		void ExecutePlayerToPool(const HashString &res, wxInt32 targetAmount)
		{
			wxInteractiveImageStrip* pool = GetResourcePool();
			wxInteractiveImageStrip* player = GetLeftSide();

			wxInt32 playerAmount = player->GetResourceCount(res);
			wxASSERT(0 < playerAmount);

			// Determine the trade ratio.
			DataObject input(res, playerAmount), output;
			RULE.Decide(shLogicIsResourceBankTradeable, input, output);
			wxASSERT(2 <= output.numItems());
			wxASSERT(true == output.read<bool>());

			wxInt32 ratio = output.read<wxInt32>(1);

			// Subtract that amount from the player and add it to the pool.
			if(0 == playerAmount - ratio)
			{
				player->Remove(res);
			}
			else
			{
				player->SetResourceCount(res, playerAmount - ratio);
			}

			pool->SetResourceCount(res, targetAmount + ratio);

			--mLeftSide;
		}

		void EnableButton()
		{
			// Enable the trade button when possible.
			GetButton()->Enable(
				(mLeftSide) && (mRightSide) && (mLeftSide == mRightSide));
		}

		// The number of valid items on each side.  They must be equal and not
		// zero for the trade to go through.
		wxInt32 mLeftSide;
		wxInt32 mRightSide;
	};

	boost::scoped_ptr<BankTradeLogic> spBankTradeLogic;
	wxInteractiveImageStrip* spPlayerBank;
	wxInteractiveImageStrip* spBank;
	wxInteractiveImageStrip *spPoolBank;
	wxBaseButton* spTradeBankButton = NULL;

	class Sizer_BankTrade : public ISizer
	{
	public:
		Sizer_BankTrade(wxWindow *window, boost::weak_ptr<Game> game) 
			: ISizer(window), mGame(game) {}

		void CreateLayout(wxBoxSizer *sizer)
		{
			wxBoxSizer *vert1 = new wxBoxSizer(wxVERTICAL);

			spBankTradeLogic.reset(new BankTradeLogic(mGame));

			wxBaseStaticBox *box = new wxBaseStaticBox(mpWindow, 0, stYou,
				wxDefaultPosition, wxDefaultSize, shDefaultText, 
				shTradeBoxFont);
			box->SetSecondLabel(stBank);
			wxStaticBoxSizer *boxSizer = 
				new wxStaticBoxSizer(box, wxHORIZONTAL);
	
			// Create the trade image strips.
			spPlayerBank = new wxSelectionImageStrip(mpWindow, 0, 3, 10, 
				spBankTradeLogic.get());
			spPlayerBank->SetFlags(wxImageStrip::FlagReverseOrder);

			spBank = new wxSelectionImageStrip(mpWindow, 0, 3, 10, 
				spBankTradeLogic.get());

			spTradeBankButton = new wxBaseButton(mpWindow, ID_TRADE_BANK_BUTTON, 
				stTradeExclamation);

			boxSizer->Add(spPlayerBank, 1, wxEXPAND | wxLEFT | wxTOP, 2);
			boxSizer->Add(spTradeBankButton, 0, 
				wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT | wxBOTTOM, 4);
			boxSizer->Add(spBank, 1, wxEXPAND | wxRIGHT | wxTOP, 2);

			spPoolBank = new wxResourcePoolImageStrip(mpWindow, 0, mGame, 
				spBankTradeLogic.get());

			// If we're in auto-trade mode, set up what they have to trade for.
			const GamePtr& thisGame = mGame.lock();
			const HashString& logic = thisGame->read<HashString>(shAutoTradeLogic);
			if(false == logic.empty())
			{
				spBank->SetFlags(wxImageStrip::FlagDisabled);

				DataObject input(0), output;
				RULE.Decide(logic, input, output);

				DataObject thisOutput;
				RULE.Decide(shLogicCanAutoTrade, output, thisOutput);

				typedef std::map<HashString, wxInt32> ResourceMap;
				typedef std::pair<ResourceMap, ResourceMap> ResourcePair;

				const ResourcePair& pair = thisOutput.read<ResourcePair>(2);

				const ResourceMap& needed = pair.first;

				ResourceMap::const_iterator it, itEnd = needed.end();
				for(it = needed.begin(); it != itEnd; ++it)
				{
					const HashString& res = it->first;
					const wxInt32 amount = it->second;

					spBank->Add(res, amount);
				}

				// They are only allowed to trade from the approved list.
				const ResourceMap& inhand = pair.second;
				spPoolBank->Clear();
				spPoolBank->SetMaxImages(inhand.size());
				itEnd = inhand.end();
				for(it = inhand.begin(); it != itEnd; ++it)
				{
					const HashString& res = it->first;
					const wxInt32 amount = it->second;

					spPoolBank->Add(res, amount);
				}
			}

			spBankTradeLogic->SetParameters(spPlayerBank, spBank, spPoolBank, 
				spTradeBankButton);

			// See if we need to force max size.
			wxInt32 width = CONFIG.read(swConfigSectionUI, swTradeWithTheBankSlash + swWidth, -1);
			wxInt32 height = CONFIG.read(swConfigSectionUI, swTradeWithTheBankSlash + swHeight, -1);

			wxInt32 initial = 0;

			if( (-1 == width) ||
				(-1 == height))
			{
				initial = -1;
			}

			// Calculate offsets.
			wxSizeEvent event(wxSize(initial, initial));
			spPlayerBank->ProcessEvent(event);
			spBank->ProcessEvent(event);
			spPoolBank->ProcessEvent(event);

			// Calculate the ratio needed to keep all cards the same size in
			// the window.
			wxInt32 poolX = spPoolBank->GetMinSize().x;
			wxInt32 boxX = boxSizer->GetMinSize().x;
			
			// Determine buffers.
			wxInt32 x = ((boxX - poolX) / 2) + 11;

			wxBoxSizer *horz2 = new wxBoxSizer(wxHORIZONTAL);
			horz2->AddSpacer(x);
			horz2->Add(spPoolBank, 1, wxEXPAND);
			horz2->AddSpacer(x);

			wxBoxSizer *horz3 = new wxBoxSizer(wxHORIZONTAL);
			wxBaseStaticText *text = new wxBaseStaticText(mpWindow, -1,
				stDragAndDropCards, shDefaultText, shInfoFont);
			
			horz3->Add(text, 1, wxEXPAND | wxALL, 2);
			
			vert1->AddSpacer(6);
			vert1->Add(boxSizer, 10, wxEXPAND | wxLEFT | wxRIGHT, 12);
			vert1->Add(horz2, 10, wxEXPAND);
			vert1->Add(horz3, 0, wxALIGN_CENTER_HORIZONTAL);

			sizer->Add(vert1, 1, wxEXPAND);
		}

	private:
		boost::weak_ptr<Game> mGame;
	};
};

//---------------------------- PUBLIC           -----------------------------//
#ifdef __WIN32__
	//disables the 'this' : used in base member initializer list warning
	#pragma warning(disable:4355)
#endif
wxBankTradeDialog::wxBankTradeDialog(wxWindow *parent, 
	boost::weak_ptr<Game> game)
: wxBaseCloseableDialog(parent, ISizerPtr(new Sizer_BankTrade(this, game)), 
	stTradeWithTheBank, swTradeWithTheBank)
, mGame(game)
{
	const GamePtr& thisGame = game.lock();
	wxASSERT(thisGame);

	// See if we're in auto-trade mode, which sets a bunch of things.
	mAutoTradeRule = thisGame->read<HashString>(shAutoTradeRule);

	if(false == mAutoTradeRule.empty())
	{
		// If we're in auto-trade mode, we have to act as a modeless dialog,
		// so hide all controls and build ui.
		Controller::get().Transmit(shEventControlsUI, DataObject(false));
		Controller::get().Transmit(shEventBuildUI, DataObject(false, 
			thisGame));
	}

	Controller::get().AddReceiver(shEventPlayerUI, 
		&wxBankTradeDialog::OnPlayerUI, this);
}

wxBankTradeDialog::~wxBankTradeDialog()
{
	spBankTradeLogic.reset();

	Controller::get().RemoveReceiver(shEventPlayerUI, 
		&wxBankTradeDialog::OnPlayerUI, this);

	Controller::get().Transmit(shEventControlsUI, DataObject(true));

	// If we're in auto-trade mode, reset things.
	if(false == mAutoTradeRule.empty())
	{
		const GamePtr& game = mGame.lock();
		wxASSERT(game);

		if(false == game->read<HashString>(shAutoTradeRule).empty())
		{
			// Only reset if we haven't been reset already.
			RULE.Execute(shRuleResetAutoTrade, DataObject(false));

			Controller::get().Transmit(shEventBuildUI, DataObject(true, game));
		}
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxBankTradeDialog, wxBaseCloseableDialog)
	EVT_BUTTON(ID_TRADE_BANK_BUTTON, wxBankTradeDialog::OnTradeButton)
END_EVENT_TABLE()

void wxBankTradeDialog::Close()
{
	// Shut down all modeless dialog boxes.
	Controller::get().Transmit(shEventShutdownModelessUI, 0);
}

void wxBankTradeDialog::OnTradeButton(wxCommandEvent &)
{
	// Disable the trade button so they can't press it multiple times 
	// before the rules get executed.
	spTradeBankButton->Disable();

	wxImageStrip::ResourceMap player, bank;

	// Create resource listings out of the two sides, then send 'em down.
	spPlayerBank->FillResourceMap(player);
	spBank->FillResourceMap(bank);

	RULE.Execute(shRuleExecuteBankTrade, DataObject(player, bank));

	// If we're in auto-trade mode, run the rule that they neede to trade for.
	if(false == mAutoTradeRule.empty())
	{
		RULE.Execute(shRuleResetAutoTrade, DataObject(true));
		Controller::get().Transmit(shEventShutdownModelessUI, 0);
		RULE.Execute(mAutoTradeRule, DataObject());
	}
    // Otherwise, they performed a straight up bank trade, which is a game action.
    else
    {
        RULE.Execute(shRulePerformGameAction, DataObject());
    }
}

void wxBankTradeDialog::OnPlayerUI(const DataObject &)
{
	// Clean everything out.
	spPlayerBank->Clear();
	spBank->Clear();
	spBankTradeLogic->Initialize();

	// If we're open when we get this event, force a redraw so that anything
	// underneath handles updates.
	Controller::get().Transmit(shEventDirty, 0);
}
