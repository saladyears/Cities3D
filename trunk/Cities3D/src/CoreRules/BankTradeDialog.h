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
// Class: wxBankTradeDialog
// 
// Handles player-initiated trades with the bank.
//
// Derived From:
//     <wxBaseCloseableDialog>
//
// Project:
//     <Cities3D>
//
// Include:
//     BankTradeDialog.h
//
class wxBankTradeDialog : public wxBaseCloseableDialog
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxBankTradeDialog
	//
	// The wxBankTradeDialog constructor.
	//
	// Parameters:
	//     parent - The parent window.
	//     game - The current <Game>.
	//
	wxBankTradeDialog(wxWindow *parent, boost::weak_ptr<Game> game);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxBankTradeDialog
	//
	// The wxBankTradeDialog destructor.
	//
	~wxBankTradeDialog();

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

	//-----------------------------------------------------------------------//
	// Group: Game Event Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnPlayerUI
	//
	// Resets the trade window.
	//
	void OnPlayerUI(const DataObject &object);

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnTradeButton
	//
	// Handles a click on the trade button, which executes the trade.
	//
	// Parameters:
	//     event - The button click event.
	//
	void OnTradeButton(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// String: mAutoTradeRule
	//
	// If we're in auto-trade mode, the rule to call after the trade is 
	// complete.
	//
	HashString mAutoTradeRule;

	//-----------------------------------------------------------------------//
	// Object: mGame
	//
	// A weak pointer to the current game.
	//
	boost::weak_ptr<Game> mGame;
};

//---------------------------- PROTOTYPES       -----------------------------//

