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

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxNetworkGamePage
// 
// Needs documentation.
//
// Derived From:
//     wxWindow
//
// Project:
//     <Cities3D>
//
// Include:
//     NetworkConnectPage.h
//
class wxNetworkGamePage : public wxPanel
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxNetworkGamePage
	//
	// The wxNetworkGamePage constructor.  Creates all UI
	// elements and lays out the page.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//
	wxNetworkGamePage(wxWindow* parent);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Constructor: ~wxNetworkGamePage
	//
	// The wxNetworkGamePage destructor.
	//
	~wxNetworkGamePage();

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
	// Updates the action button when the <Game> data changes.  If the player
	// is the host, the Start Game button becomes enabled when all players are
	// ready.  If the player is a client, the Ready button becomes enabled or
	// disabled according to their ready state.
	//
	// Parameters:
	//     game - The <Game> data.
	//
	void OnPreGame(const GamePtr &game);

	//-----------------------------------------------------------------------//
	// Function: OnResetReady
	//
	// Resets everyone's ready state.
	//
	void OnResetReady(wxInt32);

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

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
	// Function: OnActionButton
	//
	// Starts the game if the player is the server, or tells the server the 
	// player is ready if it is a client.
	//
	// Parameters:
	//     event - The button click event.
	//
	void OnActionButton(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnNotReadyButton
	//
	// Marks the player as not ready.
	//
	// Parameters:
	//     event - The button click event.
	//
	void OnNotReadyButton(wxCommandEvent &event);
};

//---------------------------- PROTOTYPES       -----------------------------//

