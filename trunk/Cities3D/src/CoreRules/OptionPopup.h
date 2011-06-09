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
#include "OptionWindow.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxScrollEvent;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxOptionPopup
// 
// A <wxSettingsDialog> option window that encapsulates UI needed for 
// popups.
//
// Derived From:
//     <wxOptionWindow>
//
// Project:
//     <Cities3D>
//
// Include:
//     OptionDisplaySize.h
//
class wxOptionPopup : public wxOptionWindow
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxOptionPopup
	//
	// The wxOptionPopup constructor.  Lays out the UI elements needed by 
	// the option window.
	//
	// Parameters:
	//     parent - The parent window.
	//
	wxOptionPopup(wxWindow* parent);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxOptionPopup
	//
	// The wxOptionPopup destructor.
	//
	~wxOptionPopup();

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
	// Function: OnPopupToggle
	//
	// Handles the user pressing the TAB key to toggle popups on/off.
	//
	// Parameters:
	//     flag - A bool that is true if popups are on and false if off.
	//
	void OnPopupToggle(bool flag);

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnAlwaysRadio
	//
	// Handles the always show popup button being clicked.
	//
	// Parameters:
	//     event - The command event.
	//
	void OnAlwaysRadio(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnHideRadio
	//
	// Handles the always hide popup button being clicked.
	//
	// Parameters:
	//     event - The command event.
	//
	void OnHideRadio(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnTimeCombo
	//
	// Handles a change in the popup time combo box.
	//
	// Parameters:
	//     event - The selection change event.
	//
	void OnTimeCombo(wxCommandEvent &event);
};

//---------------------------- PROTOTYPES       -----------------------------//
