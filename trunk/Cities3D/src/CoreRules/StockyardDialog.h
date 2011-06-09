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
#pragma once

#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//
#include "BaseGameDialog.h"
#include "TradeOffer.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class DataObject;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxStockyardDialog
// 
// The stockyard window in player to player trades.
//
// Derived From:
//     <wxBaseGameDialog>
//
// Project:
//     <Cities3D>
//
// Include:
//     StockyardDialog.h
//
class wxStockyardDialog : public wxBaseGameDialog
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxStockyardDialog
	//
	// The wxStockyardDialog constructor.
	//
	// Parameters:
	//     parent - The parent window of the dialog.
	//     game - The current <Game>.
	//     index - The index of the player at this computer.
	//
	wxStockyardDialog(wxWindow *parent, boost::weak_ptr<Game> game, 
		wxInt32 index);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxStockyardDialog
	//
	// The wxStockyardDialog destructor.
	//
	virtual ~wxStockyardDialog();

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Macro: DECLARE_EVENT_TABLE
	//
	// The wxWidgets event table declaration.
	//
	DECLARE_EVENT_TABLE()
private:
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function OnSkin
	//
	// Handles a skin change.
	//
	virtual void OnSkin(wxInt32 = 0);

	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnTrade
	//
	// Handles a trade being executed between the current player and a trade
	// partner.
	//
	// Parameters:
	//     partner - The trade partner.
	//
	void OnTrade(wxInt32 partner);

	//-----------------------------------------------------------------------//
	// Function: OnCurrentOffer
	//
	// Handles a new current offer from the trader.
	//
	// Parameters:
	//     object - The <DataObject> containing the trade offer information.
	//
	void OnCurrentOffer(const DataObject& object);
	
	//-----------------------------------------------------------------------//
	// Function: OnPlayerOffer
	//
	// Handles a new player offer from a tradee.
	//
	// Parameters:
	//     object - The <DataObject> containing the trade offer information.
	//
	void OnPlayerOffer(const DataObject& object);

	//-----------------------------------------------------------------------//
	// Function: OnAcceptOffer
	//
	// Handles a player accepting the current trade offer.
	//
	// Parameters:
	//     player - The index of the player accepting the offer.
	//
	void OnAcceptOffer(wxInt32 player);
	
	//-----------------------------------------------------------------------//
	// Function: OnRejectOffer
	//
	// Handles a player rejecting the current trade offer.
	//
	// Parameters:
	//     player - The index of the player rejecting the offer.
	//
	void OnRejectOffer(wxInt32 player);

	//-----------------------------------------------------------------------//
	// Function: OnClearOffer
	//
	// Handles a player backing out of an accepted trade.
	//
	// Parameters:
	//     player - The index of the player backing out of the trade.
	//
	void OnClearOffer(wxInt32=0);

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnPaint
	//
	// Paints the background around any trade offer highlighted.
	//
	void OnPaint(wxPaintEvent& event);
	
	//-----------------------------------------------------------------------//
	// Function: OnLeftDoubleClick
	//
	// Shortcut for automatically accepting or matching an offer.
	//
	void OnLeftDoubleClick(wxMouseEvent& event);

	//-----------------------------------------------------------------------//
	// Function: OnLeftUp
	//
	// Selects a trade offer for trade or matching.
	//
	void OnLeftUp(wxMouseEvent& event);

	//-----------------------------------------------------------------------//
	// Function: OnMouseMove
	//
	// Handles trade offer highlighting.
	//
	void OnMouseMove(wxMouseEvent& event);

	//-----------------------------------------------------------------------//
	// Function: OnAcceptButton
	//
	// Accepts the current trade offer.
	//
	void OnAcceptButton(wxCommandEvent& event);
	
	//-----------------------------------------------------------------------//
	// Function: OnRejectButton
	//
	// Rejects the current trade offer.
	//
	void OnRejectButton(wxCommandEvent& event);
	
	//-----------------------------------------------------------------------//
	// Function: OnWontTradeButton
	//
	// Marks the player as unwilling to trade.
	//
	void OnWontTradeButton(wxCommandEvent& event);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//
	
	typedef boost::shared_ptr<TradeOffer> TradeOfferPtr;
	typedef std::map<wxInt32, TradeOfferPtr> TradeOfferMap;
	
	//-----------------------------------------------------------------------//
	// Object: mTradeOffers
	//
	// The trade offer windows.
	//
	TradeOfferMap mTradeOffers;

	//-----------------------------------------------------------------------//
	// Object: mGame
	//
	// The current game.
	//
	boost::weak_ptr<Game> mGame;

	//-----------------------------------------------------------------------//
	// Int: mSelection
	//
	// The currently highlighted trade offer.
	//
	wxInt32 mSelection;

	//-----------------------------------------------------------------------//
	// Int: mPlayerIndex
	//
	// The index of the player at this computer.
	//
	wxInt32 mPlayerIndex;

	//-----------------------------------------------------------------------//
	// Int: mLastClick
	//
	// The index of the trade offer last clicked.
	//
	wxInt32 mLastClick;

	//-----------------------------------------------------------------------//
	// Int: mLastDoubleClick
	//
	// The index of the trade offer last double-clicked.
	//
	wxInt32 mLastDoubleClick;

	//-----------------------------------------------------------------------//
	// Bool: mTrader
	//
	// True if the player at this computer is the trader, false if not.
	//
	bool mTrader;
	
	//-----------------------------------------------------------------------//
	// Bool: mWontTrade
	//
	// True if the player at this computer won't trade, false if they will.
	//
	bool mWontTrade;

	//-----------------------------------------------------------------------//
	// Bool: mCanAccept
	//
	// True if the player at this computer can accept the current offer, false
	// if they cannot.
	//
	bool mCanAccept;
};

//---------------------------- PROTOTYPES       -----------------------------//

