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
#include "BaseCloseableDialog.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//
class DataObject;

//---------------------------------------------------------------------------//
// Class: wxOthersTradeDialog
// 
// Handles player-initiated trades with other players.
//
// Derived From:
//     <wxBaseCloseableDialog>
//
// Project:
//     <Cities3D>
//
// Include:
//     OthersTradeDialog.h
//
class wxOthersTradeDialog : public wxBaseCloseableDialog
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxOthersTradeDialog
	//
	// The wxOthersTradeDialog constructor.
	//
	// Parameters:
	//     parent - The parent window.
	//     game - The current <Game>.
	//     index - The index of the player at this computer.
	//
	wxOthersTradeDialog(wxWindow *parent, boost::weak_ptr<Game> game, 
		wxInt32 index);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxOthersTradeDialog
	//
	// The wxOthersTradeDialog destructor.
	//
	~wxOthersTradeDialog();

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Macro: DECLARE_EVENT_TABLE
	//
	// The wxWidgets event table declaration.
	//
	DECLARE_EVENT_TABLE()

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Close
	//
	// Closes the window.
	//
	virtual void Close();

	void OnTimer();

	//-----------------------------------------------------------------------//
	// Group: Game Event Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnPlayerUI
	//
	// Resets the trade window.
	//
	void OnPlayerUI(const DataObject &object);

	void OnClickOffer(const DataObject& object);

	void OnAcceptOffer(wxInt32 player);

	void OnRejectOffer(wxInt32 player);

	void OnClearOffer(wxInt32=0);

	void OnMatchOffer(const DataObject& object);

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnOfferButton
	//
	// Handles a click on the offer button, which makes the offer.
	//
	// Parameters:
	//     event - The button click event.
	//
	void OnOfferButton(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnTradeButton
	//
	// Handles a click on the trade button, which accepts the trade.
	//
	// Parameters:
	//     event - The button click event.
	//
	void OnTradeButton(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnRejectButton
	//
	// Handles a click on the reject button, which rejects the trade.
	//
	// Parameters:
	//     event - The button click event.
	//
	void OnRejectButton(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Object: mGame
	//
	// A weak pointer to the current game.
	//
	boost::weak_ptr<Game> mGame;

	wxInt32 mPlayerIndex;
	wxInt32 mPartnerIndex;

	bool mTrader;

	bool mPlayerAccept;
	bool mPartnerAccept;
	bool mInTimer;
};

//---------------------------- PROTOTYPES       -----------------------------//

