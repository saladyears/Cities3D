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
#include <wx/listctrl.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "PaneMixin.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxGameSetupPane
// 
// Pane that sets up a hotseat game.  Allows the user to select players and 
// assign them to colors, select a map, and apply any available options to 
// the rules.
//
// Derived From:
//     wxPanel
//     <PaneMixin>
//
// Project:
//     <CoreRules>
//
// Include:
//     GameSetupPane.h
//
class wxGameSetupPane 
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
	// Constructor: wxGameSetupPane
	//
	// The wxGameSetupPane constructor.
	//
	// Parameters:
	//     parent - The parent window.
	//
	wxGameSetupPane(wxWindow *parent);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxGameSetupPane
	//
	// The wxGameSetupPane destructor.
	//
	~wxGameSetupPane();

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
	// Function: OnPreGame
	//
	// Updates the pane.  Triggered by the eventPreGame <Event>.  Enables the 
	// OK button if there is at least one player in the game and if a map has 
	// been selected.  Calls OnPlayerSelected to refresh the current player 
	// selection.
	//
	// Parameters:
	//     game - The current <Game> object.
	//
	void OnPreGame(const GamePtr &game);

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnAddPlayer
	//
	// Runs the <wxAddPlayerDialog> window.  If the
	// user adds a new player, adds that player
	// to the <wxPlayerDropListCtrl>.
	//
	// Parameters:
	//     event - The button click event.
	//
	void OnAddPlayer(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnDeletePlayer
	//
	// Removes the player from the player database.
	//
	// Parameters:
	//     event - The button click event.
	//
	void OnDeletePlayer(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnSelectMap
	//
	// Runs the <wxSelectMapDialog> window.
	//
	// Parameters:
	//     event - The button click event.
	//
	void OnSelectMap(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnGameOptions
	//
	// Fires up the <wxOptionsDialog> window.
	//
	// Parameters:
	//     event - The button click event.
	//
	void OnGameOptions(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnPlayerSelected
	//
	// Handles a player selection in the <wxPlayerDropListCtrl>.
	// Enables the Edit button if a player is selected,
	// disables it if not.
	//
	// Parameters:
	//     event - The list selection event.
	//
	void OnPlayerSelected(wxListEvent &event);
	
	//-----------------------------------------------------------------------//
	// Function: OnOK
	//
	// Handles the player pressing the OK button to start the game.
	//
	void OnOK(wxCommandEvent&);
};
//---------------------------- PROTOTYPES       -----------------------------//

