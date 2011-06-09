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
#include "PaneMixin.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxListEvent;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxOpenGamePane
// 
// Dialog that allows users to open a saved game.
//
// Derived From:
//     wxPanel
//     <PaneMixin>
//
// Project:
//     <CoreRules>
//
// Include:
//     OpenGamePane.h
//
class wxOpenGamePane 
	: public wxPanel
	, public PaneMixin
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxSelectMapDialog
	//
	// The wxOpenGamePane constructor.
	//
	// Parameters:
	//     parent - The parent window.
	//
	wxOpenGamePane(wxWindow *parent);

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
	// Function: Ok
	//
	// Handles the OK button click event.
	//
	void OnOk(wxCommandEvent&);

	//-----------------------------------------------------------------------//
	// Function: OnGameDoubleClick
	//
	// Handles a double-click in the <wxGameListCtrl>.
	// Immediately calls Ok to exit the dialog.  Shortcut
	// instead of having to select a game and click the 
	// OK button.
	//
	// Parameters:
	//     event - The list selection event.
	//
	void OnGameDoubleClick(wxListEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnGameSelected
	//
	// Handles a selection or unselection in the <wxGameListCtrl>.
	// Enables or disables the OK button.
	//
	// Parameters:
	//     event - The list selection event.
	//
	void OnGameSelected(wxListEvent &event);
};

//---------------------------- PROTOTYPES       -----------------------------//

