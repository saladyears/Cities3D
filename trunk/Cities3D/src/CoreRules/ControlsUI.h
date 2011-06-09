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
#include "BaseUI.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxBaseButton;
class DataObject;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxControlsUI
// 
// Displays a set of buttons for user turn actions.  wxControlsUI sits at
// the top of the screen just to the left of the <MessageUI>.
// It holds action buttons that players can use on their turn to
// undo an action (if possible), trade with each other, trade 
// with the bank, play a card, or end their turn.  wxControlsUI can
// be updated by transmitting a <DataObject> to the eventControlsUI
// <Event> via the <Controller>.  The transmitted <DataObject> must
// have a bool as its first parameter.  If the bool value is false, 
// the wxControlsUI hides itself; if the bool value is true, the 
// wxControlsUI shows itself and updates each of its buttons 
// via <ILogic> rules.  The buttons are enabled or disabled depending
// on the result of the <ILogic>.  If a user clicks an enabled button,
// wxControlsUI calls the <RuleEngine> with an <IRule> to execute.
//
// Derived From:
//     <wxBaseUI>
//
// Project:
//     <Cities3D>
//
// Include:
//     wxControlsUI.h
//
class wxControlsUI : public wxBaseUI<wxPanel>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxControlsUI
	//
	// The wxControlsUI constructor.  Sets the resize window
	// that the wxControlsUI depends on to calculate its offset (should
	// be the MessageUI window), and adds OnUpdate as a receiver
	// of the eventControlsUI <Event>.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//
	wxControlsUI(wxWindow *parent);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxControlsUI
	//
	// The wxControlsUI destructor.  Removes OnUpdate as a receiver
	// of the eventControlsUI <Event>.
	//
	~wxControlsUI();
	
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
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnUpdate
	//
	// Updates the wxControlsUI.  Triggered by the eventControlsUI
	// <Event>.  The given <DataObject> must have a bool as its
	// first parameter.  If the bool is true, the wxControlsUI is shown
	// on the screen, if false, the wxControlsUI is hidden.  Calls
	// hard-coded <Logic> for each of the buttons, to determine
	// if the button should be enabled.  Enables or disables the
	// buttons appropriately.  If any of the buttons are enabled,
	// the entire wxControlsUI is set to clickable state (and drawn with
	// a white outline), otherwise, it is set to not clickable and
	// drawn with the standard outline.
	//
	// Parameters:
	//     object - The <DataObject> containing the update information.
	//              Must have a bool as its first parameter.
	//
	void OnUpdate(const DataObject &object);

	//-----------------------------------------------------------------------//
	// Function: OnUpdateUndo
	//
	// Updates the Undo button by checking the <IRuleEngine>.
	//
	void OnUpdateUndo(wxInt32);

	//-----------------------------------------------------------------------//
	// Function: OnShutdownUI
	//
	// Turns off all options on a global shutdown.
	//
	void OnShutdownUI(wxInt32);

	//-----------------------------------------------------------------------//
	// Function: OnPreRollCard
	//
	// Handles the situation when a player has a card they can play pre-roll.
	//
	// Parameters:
	//     show - A flag that is true if the player is in pre-roll state and
	//            false if not.
	//
	void OnPreRollCard(bool show);

	//-----------------------------------------------------------------------//
	// Function: OnControlsUndo
	//
	// Handles the key map event for Undo Action.
	//
	void OnControlsUndo(wxInt32);

	//-----------------------------------------------------------------------//
	// Function: OnControlsTrade
	//
	// Handles the key map event for Trade with others.
	//
	void OnControlsTrade(wxInt32);

	//-----------------------------------------------------------------------//
	// Function: OnControlsBank
	//
	// Handles the key map event for Trade with bank.
	//
	void OnControlsBank(wxInt32);

	//-----------------------------------------------------------------------//
	// Function: OnControlsPlayCard
	//
	// Handles the key map event for Play card.
	//
	void OnControlsPlayCard(wxInt32);

	//-----------------------------------------------------------------------//
	// Function: OnControlsRollDice
	//
	// Handles the key map event for Roll dice.
	//
	void OnControlsRollDice(wxInt32);

	//-----------------------------------------------------------------------//
	// Function: OnControlsEndTurn
	//
	// Handles the key map event for End turn.
	//
	void OnControlsEndTurn(wxInt32);

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnButton
	//
	// Handles any action button click.  Calls the <RuleEngine>
	// with a hard-coded <Rule> for each of the buttons.
	//
	// Parameters:
	//     event - The button click event.
	//
	void OnButton(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	typedef boost::shared_ptr<wxBaseButton> ClearButtonPtr;
	typedef std::vector<ClearButtonPtr> ClearButtonArray;

	//-----------------------------------------------------------------------//
	// Array: mButtons
	//
	// The action buttons.  Created as <wxClearButtons> for
	// a common UI look and feel.
	//
	ClearButtonArray mButtons;
};

//---------------------------- PROTOTYPES       -----------------------------//

