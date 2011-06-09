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

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxOptionSounds
// 
// A <wxSettingsDialog> option window that encapsulates 
// UI needed for changing the sounds used in the game.
//
// Derived From:
//     <wxOptionWindow>
//
// Project:
//     <Cities3D>
//
// Include:
//     OptionImages.h
//
class wxOptionSounds : public wxOptionWindow
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxOptionSounds
	//
	// The wxOptionSounds constructor.  Lays out the
	// UI elements needed by the option window.
	//
	// Parameters:
	//     parent - The parent window.
	//
	wxOptionSounds(wxWindow* parent);

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
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnSoundsComboSel
	//
	// Handles a change in the Sounds combo box.  Sets the
	// new sounds subfolder.
	//
	// Parameters:
	//     event - The selection change event.
	//
	void OnSoundsComboSel(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnSoundsCheck
	//
	// Handles a click on the play sounds checkbox.  Sets the
	// new value in the <ConfigDatabase>.
	//
	void OnSoundsCheck(wxCommandEvent &event);
};

//---------------------------- PROTOTYPES       -----------------------------//
