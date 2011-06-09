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
class wxTreeEvent;
class wxOptionWindow;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxSettingsPane
// 
// Pane that allows users to pick from a selection of configurable
// settings arranged in a tree control.  Each set of configurable options
// is laid out in a <wxOptionWindow>-derived class.  When the user clicks
// on a node in the tree, that <wxOptionWindow> is made visible and laid
// out in the available space in the wxSettingsPane, which allows the user
// to change the settings relevant for that option, then select more options
// to change by clicking another node in the tree.
//
// Derived From:
//     wxPanel
//     <PaneMixin>
//
// Project:
//     <Cities3D>
//
// Include:
//     SettingsPane.h
//
class wxSettingsPane 
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
	// Constructor: wxSettingsPane
	//
	// The wxSettingsPane constructor.  Creates the tree
	// control and all child <wxOptionWindows>.
	//
	// Parameters:
	//     parent - The parent window.
	//     id - The id of the menu controlling this pane, if any.
	//
	wxSettingsPane(wxWindow *parent, wxInt32 id = -1);

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
	// Function: OnTreeSelection
	//
	// Updates the currently displayed <wxOptionWindow> based
	// on the tree node selected by the user.
	//
	void OnTreeSelection(wxTreeEvent &event);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Pointer: mpCurrentWindow
	//
	// The currently displayed <wxOptionWindow>.  NULL if none
	// selected.
	//
	wxOptionWindow *mpCurrentWindow;
};

//---------------------------- PROTOTYPES       -----------------------------//
