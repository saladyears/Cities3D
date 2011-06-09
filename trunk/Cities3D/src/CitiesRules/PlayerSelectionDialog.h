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
#include <boost/function.hpp>

//---------------------------- USER INCLUDES    -----------------------------//
#include "BaseGameDialog.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class Game;
class wxBaseListCtrl;
class wxListEvent;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxPlayerSelectionDialog
// 
// Displays a dialog where the user can select a player for various reasons.
//
// Derived From:
//     <wxBaseGameDialog>
//
// Project:
//     <Cities3D>
//
// Include:
//     PlayerSelectionDialog.h
//
class wxPlayerSelectionDialog : public wxBaseGameDialog
{
public:
	typedef boost::function2<void, wxBaseListCtrl*, boost::weak_ptr<Game> > 
		PopulateListFunc;

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxPlayerSelectionDialog
	//
	// The wxPlayerSelectionDialog constructor.
	//
	// Parameters:
	//     parent - The parent window of the dialog.
	//     game - The current <Game>.
	//     rule - The <Rule> to execute when the button is clicked.
	//     caption - The window caption.
	//     func - The function used to populate the player list control.
	//
	wxPlayerSelectionDialog(wxWindow *parent, boost::weak_ptr<Game> game,
		const HashString& rule, const wxString& text, PopulateListFunc func);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxPlayerSelectionDialog
	//
	// The wxPlayerSelectionDialog destructor.
	//
	virtual ~wxPlayerSelectionDialog();

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
	// Function: OnButton
	//
	// Handles a click on the button, which executes the selection.
	//
	// Parameters:
	//     event - The button click event.
	//
	void OnButton(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnPlayerSelected
	//
	// Handles a selection event in the player list.
	//
	// Parameters:
	//     event - The button click event.
	//
	void OnPlayerSelected(wxListEvent& event);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// String: mRule
	//
	// The <Rule> to execute when the button is clicked.
	//
	HashString mRule;
};

//---------------------------- PROTOTYPES       -----------------------------//

