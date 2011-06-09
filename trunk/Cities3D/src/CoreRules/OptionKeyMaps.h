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
#include "KeystrokeEntryTextCtrl.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxGridEvent;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxOptionKeyMaps
// 
// A <wxSettingsDialog> option window that encapsulates UI needed for changing 
// the keymaps used in the game.
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
class wxOptionKeyMaps 
    : public wxOptionWindow
    , public IKeystrokeHandler
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxOptionKeyMaps
	//
	// The wxOptionKeyMaps constructor.  Lays out the
	// UI elements needed by the option window.
	//
	// Parameters:
	//     parent - The parent window.
	//
	wxOptionKeyMaps(wxWindow* parent);

    //-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

    //-----------------------------------------------------------------------//
	// Function: HandleKeyEvent
	//
    // Processes a key event passed from the keystroke entry control.
    //
    // Parameters:
    //     event - The key event.
    //
    virtual void HandleKeyEvent(const wxKeyEvent& event, bool keydown);

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
	// Function: OnAssign
	//
    // Handles the Assign button be clicked.
    //
    void OnAssign(wxCommandEvent&);

	//-----------------------------------------------------------------------//
	// Function: OnGridCellLeftClick
	//
	// Handles a left click in the grid.
	//
	// Parameters:
	//     event - The grid event.
	//
	void OnGridCellLeftClick(wxGridEvent& event);

	//-----------------------------------------------------------------------//
	// Function: OnGridLabelLeftClick
	//
	// Handles a left click on a grid label.
	//
	// Parameters:
	//     event - The grid event.
	//
	void OnGridLabelLeftClick(wxGridEvent& event);
};

//---------------------------- PROTOTYPES       -----------------------------//
