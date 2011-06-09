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
#include "BaseListCtrl.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxPlayerListCtrl
// 
// Displays all of the players in the <PlayerDatabase> in a list control.
// Each player is displayed by name.  The list is sorted
// in alphabetical order.
//
// Derived From:
//     <wxBaseListCtrl>
//
// Project:
//     <Cities3D>
//
// Include:
//     PlayerListCtrl.h
//
class UI_EXPORT_SPEC wxPlayerListCtrl : public wxBaseListCtrl
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxPlayerListCtrl
	// 
	// The wxPlayerListCtrl constructor.  Creates the columns for
	// the list control and then calls ImportPlayers to populate
	// the control.
	//
	// Parameters:
	//     parent - The parent window.
	//     id - The message handling ID.  Should be a unique (to the parent 
	//          window) ID, if the parent window wants to receive messages from
	//          the control.
	//     pos - The list control position in window coordinates.
	//     size - The list control size.
	//
	wxPlayerListCtrl(wxWindow* parent, wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize);
	
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: AddPlayer
	//
	// Adds a new player to the list.  Looks up the player's
	// index in the <PlayerDatabase> and then inserts the new player's
	// name into the list, storing the index for later reference.
	//
	// Parameters:
	//     name - The name of the new player.
	//
	void AddPlayer(const wxString &name);

	//-----------------------------------------------------------------------//
	// Function: EditPlayer
	//
	// Changes the name of a player currently in the list.
	//
	// Parameters:
	//     oldName - The name of the player in the list.
	//     newName - The new name of the player.
	//
	void EditPlayer(const wxString &oldName, const wxString &newName);

	//-----------------------------------------------------------------------//
	// Function: DeletePlayer
	//
	// Deletes the player at the given index.
	//
	// Parameters:
	//     index - The index of the player to delete.
	//
	void DeletePlayer(const size_t index);

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: ImportPlayers
	//
	// Reads all the players from the <PlayerDatabase> and inserts
	// the information into the list control.
	//
	void ImportPlayers();

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Macro: DECLARE_EVENT_TABLE
	//
	// The wxWidgets event table declaration.
	//
	DECLARE_EVENT_TABLE()

private:
	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnEnterWindow
	//
	// Handles the mouse entering the list's rectangle.
	// Sets the cursor to be an arrow.
	//
	// Parameters:
	//     event - The mouse event.
	//
	void OnEnterWindow(wxMouseEvent &event);
};

//---------------------------- PROTOTYPES       -----------------------------//
