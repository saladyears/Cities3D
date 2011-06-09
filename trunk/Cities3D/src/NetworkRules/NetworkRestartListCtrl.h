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
class DataObject;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxNetworkRestartListCtrl
//
// Displays all the players currently in the game (or not) when waiting for a
// network game to restart.
//
// Derived From:
//     <wxBaseListCtrl>
//
// Project:
//     <Cities3D>
//
// Include:
//     NetworkRestartListCtrl.h
//
class wxNetworkRestartListCtrl : public wxBaseListCtrl
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxNetworkRestartListCtrl
	// 
	// The wxNetworkRestartListCtrl constructor.
	//
	// Parameters:
	//     parent - The parent window.
	//     id - The message handling ID.  Should be a unique (to the parent
	//          window) ID, if the parent window wants to receive messages from
	//          the control.
	//     pos - The list control position in window coordinates.
	//     size - The list control size.
	//
	wxNetworkRestartListCtrl(wxWindow* parent, wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxNetworkRestartListCtrl
	//
	// The wxNetworkRestartListCtrl destructor.
	//
	~wxNetworkRestartListCtrl();

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnUpdatePlayers
	//
	// Sets player names for all <Players> in the <Game> at the selection with
	// their color.  Triggered by the eventNetworkRestartPlayer <Event>.
	//
	// Parameters:
	//     game - The current <Game>.
	//
	void OnUpdatePlayers(const GamePtr &game);

	//-----------------------------------------------------------------------//
	// Function: OnCountdownTime
	//
	// Updates the countdown time reamining for the given color.
	//
	// Parameters:
	//      object - The <DataObject> containing the color and time remaining.
	//
	void OnCountdownTime(const DataObject &object);
};

//---------------------------- PROTOTYPES       -----------------------------//

