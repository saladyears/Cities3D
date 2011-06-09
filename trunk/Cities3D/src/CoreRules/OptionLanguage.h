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
// Class: wxOptionLanguage
// 
// A <wxSettingsDialog> option window that encapsulates UI needed for changing 
// the language used in the game.
//
// Derived From:
//     <wxOptionWindow>
//
// Project:
//     <Cities3D>
//
// Include:
//     OptionLanguage.h
//
class wxOptionLanguage : public wxOptionWindow
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxOptionLanguage
	//
	// The wxOptionLanguage constructor.  Lays out the UI elements needed by 
	// the option window.
	//
	// Parameters:
	//     parent - The parent window.
	//
	wxOptionLanguage(wxWindow* parent);

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
	// Function: OnLanguageComboSel
	//
	// Handles a change in the language combo box.
	//
	// Parameters:
	//     event - The selection change event.
	//
	void OnLanguageComboSel(wxCommandEvent &event);
};

//---------------------------- PROTOTYPES       -----------------------------//
