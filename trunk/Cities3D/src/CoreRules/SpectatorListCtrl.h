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
// Class: wxSpectatorListCtrl
// 
// Displays all of the spectators in the current game.
//
// Derived From:
//     <wxBaseListCtrl>
//
// Project:
//     <Cities3D>
//
// Include:
//     SpectatorListCtrl.h
//
class wxSpectatorListCtrl : public wxBaseListCtrl
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxSpectatorListCtrl
	// 
	// The wxSpectatorListCtrl constructor.  Creates the columns for
	// the list control.  Adds OnUpdatePlayers to the <Controller> as 
	// a receiver of the eventSpectator <Event>.
	//
	// Parameters:
	//     parent - The parent window.
	//     id - The message handling ID.  Should be a unique (to the parent
	//          window) ID, if the parent window wants to receive messages from
	//          the control.
	//     pos - The list control position in window coordinates.
	//     size - The list control size.
	//
	wxSpectatorListCtrl(wxWindow* parent, wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxSpectatorListCtrl
	// 
	// The wxSpectatorListCtrl destructor.  Removes OnUpdatePlayers 
	// as a receiver of the eventSpectator <Event>.
	//
	virtual ~wxSpectatorListCtrl();

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnUpdateSpectators
	//
	// Repopulates the spectator list.  Triggered by the eventSpectator
	// <Event>.
	//
	void OnUpdateSpectators(wxInt32);

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: SetSpectatorColor
	//
	// Allows derived classes to set the item color in question.
	//
	// Parameters:
	//     index - The index to set.
	//
	virtual void SetSpectatorColor(const size_t) {}
};

//---------------------------- PROTOTYPES       -----------------------------//

