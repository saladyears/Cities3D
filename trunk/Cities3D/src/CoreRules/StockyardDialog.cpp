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
#include "StockyardDialog.h"
#include "ISizer.h"
#include "Game.h"
#include "TradeOfferBox.h"
#include "BaseStaticText.h"
#include "BaseButton.h"
#include "ImageStrip.h"
#include "Controller.h"
#include "DataObject.h"
#include "SkinManager.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	enum
	{
		ID_ACCEPT_BUTTON			= 0,
		ID_REJECT_STOCKYARD_BUTTON	,
		ID_WONTTRADE_BUTTON			,
	};

	wxBaseButton* spAcceptButton = NULL;
	wxBaseButton* spRejectStockyardButton = NULL;
	wxBaseButton* spWontTradeButton = NULL;
	
	wxColour sHighlight;
	wxColour sMatch;
	wxColour sBackgroundStockyard;
	wxColour sSelectionColor;

	const wxInt32 sCurrentOffer = -2;

	typedef std::map<HashString, wxInt32> ResourceMap;
}

//---------------------------- PUBLIC           -----------------------------//
#ifdef __WIN32__
	//disables the 'this' : used in base member initializer list warning
	#pragma warning(disable:4355)
#endif
wxStockyardDialog::wxStockyardDialog(wxWindow *parent, 
	boost::weak_ptr<Game> game, wxInt32 index)
: wxBaseGameDialog(parent, ISizerPtr(), stStockyard, swStockyard)
, mGame(game)
, mSelection(-1)
, mPlayerIndex(index)
, mLastClick(-1)
, mLastDoubleClick(-1)
, mTrader(false)
, mWontTrade(false)
, mCanAccept(false)
{
	OnSkin();

	const GamePtr& thisGame = game.lock();
	wxASSERT(thisGame);

	wxInt32 current = thisGame->read<wxInt32>(shCurrentPlayer);
	
	// Set the trader flag if this player is the current player.
	if(mPlayerIndex == current)
	{
		mTrader = true;
	}

	wxBoxSizer* vert = new wxBoxSizer(wxVERTICAL);

	// Create a trade offer slot for each player.
	wxInt32 players = thisGame->numPlayers();
			
	// Create the TradeOffers for each player.
	for(wxInt32 i = 0; i < players; ++i)
	{
		wxString str, str2;
		ColorType color;
		wxInt32 thisPlayer = sCurrentOffer;

		// The top TradeOffer is the offer from the trader.
		if(0 == i)
		{
			wxString name = thisGame->playerGame(current).player().Name();

			if(true == mTrader)
			{
				str = stYouOffer;
				str2 = stYouWant;
			}
			else
			{
				str = wxString::Format(stStringWants.c_str(), name.c_str());
				str2 = wxString::Format(stStringOffers.c_str(), name.c_str());
			}

			color = CR_SIZE;
		}
		// The rest of the TradeOffers correspond to the rest of the players.
		else
		{
			thisPlayer = (i - (i <= current));
			
			str = thisGame->playerGame(thisPlayer).player().Name();
			color = thisGame->playerGame(thisPlayer).color();
		}

		// Create the TradeOffer.
		wxTradeOfferBox* box = new wxTradeOfferBox(this, str, color, thisPlayer, 
			current, mPlayerIndex, str2);
	
		TradeOfferPtr offer(new TradeOffer(thisPlayer, mTrader, this, vert, 
			box));
		mTradeOffers[thisPlayer] = offer;
					
		if(0 == i)
		{
			vert->AddSpacer(8);

			// Add more buttons if this is not the trader (and not a spectator).
			if( (false == mTrader) &&
				(-1 != mPlayerIndex))
			{
				wxBoxSizer* horz = new wxBoxSizer(wxHORIZONTAL);

				spAcceptButton = new wxBaseButton(this, ID_ACCEPT_BUTTON, stAccept);
				spAcceptButton->Disable();
				spRejectStockyardButton = new wxBaseButton(this, ID_REJECT_STOCKYARD_BUTTON, 
					stReject);
				spRejectStockyardButton->Disable();
				spWontTradeButton = new wxBaseButton(this, ID_WONTTRADE_BUTTON, 
					stNoTrades);
				spWontTradeButton->Disable();

				horz->AddStretchSpacer();
				horz->Add(spAcceptButton, 0, wxRIGHT, 4);
				horz->Add(spRejectStockyardButton, 0, wxLEFT | wxRIGHT, 4);
				horz->Add(spWontTradeButton, 0, wxLEFT, 4);
				horz->AddStretchSpacer();

				vert->Add(horz, 0, wxEXPAND | wxALL, 4);
			}

			// Add explanatory text for all players (but not spectators).
			if(-1 != mPlayerIndex)
			{
				wxString str = mTrader ? stClickOnAnOfferToTrade : stClickOtherOffers;

				wxBaseStaticText* text = new wxBaseStaticText(this, -1, 
					str, shDefaultText, shInfoFont);
				vert->Add(text, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP | 
					wxBOTTOM, 4);
			}
		}
	
		vert->AddSpacer(4);
	}
	
	GetSizer()->Add(vert, 1, wxEXPAND);

	wxSize minSize = GetSizer()->GetMinSize();
	wxSize thisSize = GetSize();

	bool resize = false;
	if(thisSize.x < minSize.x)
	{
		thisSize.x = minSize.x;
		resize = true;
	}
	if(thisSize.y < minSize.y)
	{
		thisSize.y = minSize.y;
		resize = true;
	}

	if(true == resize)
	{
		SetSize(thisSize);
	}

	Controller::get().AddReceiver(shEventTrade, &wxStockyardDialog::OnTrade, 
		this);
	Controller::get().AddReceiver(shEventCurrentOffer, 
		&wxStockyardDialog::OnCurrentOffer, this);
	Controller::get().AddReceiver(shEventPlayerOffer,
		&wxStockyardDialog::OnPlayerOffer, this);
	Controller::get().AddReceiver(shEventAcceptOffer,
		&wxStockyardDialog::OnAcceptOffer, this);
	Controller::get().AddReceiver(shEventRejectOffer,
		&wxStockyardDialog::OnRejectOffer, this);
	Controller::get().AddReceiver(shEventClearOffer,
		&wxStockyardDialog::OnClearOffer, this);

	sSelectionColor = sBackgroundStockyard;
}

wxStockyardDialog::~wxStockyardDialog()
{
	spAcceptButton = NULL;
	spRejectStockyardButton = NULL;
	spWontTradeButton = NULL;

	Controller::get().RemoveReceiver(shEventTrade, &wxStockyardDialog::OnTrade, 
		this);
	Controller::get().RemoveReceiver(shEventCurrentOffer, 
		&wxStockyardDialog::OnCurrentOffer, this);
	Controller::get().RemoveReceiver(shEventPlayerOffer,
		&wxStockyardDialog::OnPlayerOffer, this);
	Controller::get().RemoveReceiver(shEventAcceptOffer,
		&wxStockyardDialog::OnAcceptOffer, this);
	Controller::get().RemoveReceiver(shEventRejectOffer,
		&wxStockyardDialog::OnRejectOffer, this);
	Controller::get().RemoveReceiver(shEventClearOffer,
		&wxStockyardDialog::OnClearOffer, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxStockyardDialog, wxBaseGameDialog)
	EVT_PAINT(wxStockyardDialog::OnPaint)
	EVT_LEFT_DCLICK(wxStockyardDialog::OnLeftDoubleClick)
	EVT_LEFT_UP(wxStockyardDialog::OnLeftUp)
	EVT_MOTION(wxStockyardDialog::OnMouseMove)
	EVT_BUTTON(ID_ACCEPT_BUTTON, wxStockyardDialog::OnAcceptButton)
	EVT_BUTTON(ID_REJECT_STOCKYARD_BUTTON, wxStockyardDialog::OnRejectButton)
	EVT_BUTTON(ID_WONTTRADE_BUTTON, wxStockyardDialog::OnWontTradeButton)
END_EVENT_TABLE()

void wxStockyardDialog::OnSkin(wxInt32)
{
	sMatch = SKIN.Element(shTradeMatch);
	sHighlight = SKIN.Element(shTradeClick);
	sBackgroundStockyard = SKIN.Element(shBackground);

	wxBaseGameDialog::OnSkin();
}

void wxStockyardDialog::OnTrade(wxInt32)
{
	mLastClick = -1;
	mLastDoubleClick = -1;

	// Enable the won't trade button in case they disabled it.
	if( (false == mTrader) &&
		(-1 != mPlayerIndex))
	{
		spWontTradeButton->Enable(!mWontTrade);
	}
}

void wxStockyardDialog::OnCurrentOffer(const DataObject& object)
{
	// If we are not the trader, enable/disable the buttons.
	if( (false == mTrader) &&
		(-1 != mPlayerIndex))
	{
		// Reset the last clicked if there's a new offer.
		if(sCurrentOffer == mLastClick)
		{
			mLastClick = -1;
		}

		const ResourceMap& left = object.read<ResourceMap>(1);
		const ResourceMap& right = object.read<ResourceMap>();

		// If the person is marked as won't trade, the only thing they can
		// do is accept the trade (or make a counteroffer).
		spRejectStockyardButton->Enable(!mWontTrade);
		spWontTradeButton->Enable(!mWontTrade);

		// See if they can make the trade as is.
		const GamePtr& game = mGame.lock();
		const Data::IntHash& resources = 
			game->playerGame(mPlayerIndex).read<Data::IntHash>(shResources);
		mCanAccept = true;
		
		ResourceMap::const_iterator it, itEnd = left.end();
		for(it = left.begin(); it != itEnd; ++it)
		{
			const HashString& res = it->first;
			wxInt32 amount = it->second;
			wxInt32 playerAmount = 0;

			if(shAnyMatch != res)
			{
				// See if they have it in enough quantities.
				Data::IntHash::const_iterator itRes = resources.find(res);
				wxASSERT(resources.end() != itRes);

				playerAmount = itRes->second;
			}

			if(amount > playerAmount)
			{
				mCanAccept = false;
				break;
			}
		}

		// If they've gotten this far, test the right side of the trade for
		// any match cards.
		if( (true == mCanAccept) &&
			(right.end() != right.find(shAnyMatch)))
		{
			mCanAccept = false;
		}

		spAcceptButton->Enable(mCanAccept);
	}

	Refresh();
	Show();
}

void wxStockyardDialog::OnPlayerOffer(const DataObject& object)
{
	wxInt32 player = object.read<wxInt32>(2);

	// Reset the last clicks if there's a new offer.
	if(player == mLastClick)
	{
		mLastClick = -1;
	}

    if(player == mLastDoubleClick)
    {
        mLastDoubleClick = -1;
    }

	// If this is the player at this seat (i.e., the made a counter offer),
	// reset their wont accept state.
	if(player == mPlayerIndex)
	{
		// Since they decided to do something, they are obviously back in the
		// trading game.
		mWontTrade = false;
		spWontTradeButton->Enable();

		// If they matched the current offer, they accepted it and their accept
		// button should be disabled.  It is possible that they matched the
		// offer from the wxOthersTradeDialog window, which means we can't 
		// assume that the accept button was already disabled.
		TradeOfferPtr playerOffer = mTradeOffers[player];
		TradeOfferPtr currentOffer = mTradeOffers[sCurrentOffer];

		if(true == playerOffer->CompareOffer(*currentOffer))
		{
			spAcceptButton->Disable();
		}
		// Otherwise, they are making a counteroffer.  In this case, the reject
		// button should be disabled because they have taken responsibility for
		// their own trades.
		else
		{
			spRejectStockyardButton->Disable();
		}
	}
}

void wxStockyardDialog::OnAcceptOffer(wxInt32 player)
{
	// If the player decides to accept an offer, they cannot refuse to trade
	// until the trader decides what to do with the offer.
	if( (false == mTrader) &&
		(mPlayerIndex == player))
	{
		spWontTradeButton->Disable();
	}
}

void wxStockyardDialog::OnRejectOffer(wxInt32 player)
{
	// If this is the player rejecting, then they may be able to accept again.
	if( (false == mTrader) &&
		(mPlayerIndex == player))
	{
		spAcceptButton->Enable(mCanAccept);
	}
}

void wxStockyardDialog::OnClearOffer(wxInt32)
{
	// Enable the won't trade button in case they disabled it.
	if(false == mTrader)
	{
		if(-1 != mPlayerIndex)
		{
			spWontTradeButton->Enable(!mWontTrade);
		}
	}
	else
	{
		mLastClick = -1;
		mLastDoubleClick = -1;
	}
}

void wxStockyardDialog::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	wxASSERT(TRUE == dc.Ok());

	// Draw the selection box if needed.
	if(-1 != mSelection)
	{
		dc.SetBrush(wxBrush(sSelectionColor));
		dc.SetPen(wxPen(sSelectionColor));

		wxRect rect = mTradeOffers[mSelection]->GetSizerRect();
		wxSize thisSize = GetSize();

		dc.DrawRectangle(0, rect.y - 4, thisSize.x, rect.height + 8);
	}	

	event.Skip();
}

void wxStockyardDialog::OnLeftDoubleClick(wxMouseEvent&)
{
	if(mLastDoubleClick != mLastClick)
	{
		// If the trader is double-clicking something, it means they want to
		// accept that trade offer.
		if(true == mTrader)
		{
			if( (sHighlight == sSelectionColor) && (-1 != mSelection) )
			{
				// A double-click is a normal click with an automatic accept.
				RULE.Execute(shRuleAcceptOffer, DataObject(mPlayerIndex));
			}
		}
		// Otherwise, it means that the player wants to match the selected 
		// offer.
		else if( (-1 != mPlayerIndex) && (-1 != mSelection) )
		{
			// For tradees, if they double-click another players' offer, they 
			// automatically make the offer if they can match it.
			mTradeOffers[mSelection]->MatchOffer(mPlayerIndex);
		}

		mLastDoubleClick = mLastClick;
	}
}

void wxStockyardDialog::OnAcceptButton(wxCommandEvent&)
{
	TradeOffer::AcceptOffer(mPlayerIndex);

	// Clear buttons.
	spAcceptButton->Disable();
	spRejectStockyardButton->Enable();
	spWontTradeButton->Enable();

	mWontTrade = false;
}

void wxStockyardDialog::OnRejectButton(wxCommandEvent&)
{
	RULE.Execute(shRuleRejectOffer, DataObject(mPlayerIndex));

	// Clear buttons.
	spRejectStockyardButton->Disable();
}

void wxStockyardDialog::OnWontTradeButton(wxCommandEvent&)
{
	mWontTrade = true;

	RULE.Execute(shRuleWontTrade, DataObject(mPlayerIndex));

	// Clear buttons.
	spAcceptButton->Enable(mCanAccept);
	spRejectStockyardButton->Disable();
	spWontTradeButton->Disable();
}

void wxStockyardDialog::OnLeftUp(wxMouseEvent&)
{
	if(-1 != mSelection)
	{
		DataObject object;

		mTradeOffers[mSelection]->FillOffer(object);
		
		// Send it.
		if(true == mTrader)
		{
			// Test for match vs. accept.
			if(sHighlight == sSelectionColor)
			{
				// Avoid resending twice after a double-click.
				if( (-1 == mLastClick) ||
					(mLastClick != mLastDoubleClick))
				{
					RULE.Execute(shRuleClickOffer, object);
				}
			}
			else if(sMatch == sSelectionColor)
			{
				Controller::get().Transmit(shEventMatchOffer, object);
			}
		}
		// They are matching an offer.
		else
		{
			Controller::get().Transmit(shEventMatchOffer, object);
		}

		mLastClick = mSelection;
	}
}

void wxStockyardDialog::OnMouseMove(wxMouseEvent& event)
{
	wxPoint pos = event.GetPosition();

	wxInt32 found = -1;

	// See if the mouse point is inside any selections.
	TradeOfferMap::const_iterator it, itEnd = mTradeOffers.end();
	for(it = mTradeOffers.begin(); it != itEnd; ++it)
	{
		TradeOfferPtr window = it->second;
		
		bool canTrade;
		found = window->TestClick(pos, mPlayerIndex, canTrade);

		// If they are over a selection, it should be highlighted, unless
		// it is the trader hovering over the current offer.
		if( (0 <= found) ||
			((-2 == found) &&
			(false == mTrader)))
		{
			sSelectionColor = canTrade ? sHighlight : sMatch;
			break;
		}
	}

	// If the current selection has changed, updated the background colors
	// and redraw.
	if(found != mSelection)
	{
		if(-1 != mSelection)
		{
			TradeOfferPtr window = mTradeOffers[mSelection];
			window->SetBackgroundColour(sBackgroundStockyard);
		}

		if(-1 != found)
		{
			TradeOfferPtr window = mTradeOffers[found];
			window->SetBackgroundColour(sSelectionColor);
		}

		mSelection = found;
		Refresh();
	}

	if(-1 != mSelection)
	{
		SetCursor(wxCursor(wxCURSOR_HAND));
	}
	else
	{
		SetCursor(wxCursor(wxCURSOR_ARROW));
	}
}
