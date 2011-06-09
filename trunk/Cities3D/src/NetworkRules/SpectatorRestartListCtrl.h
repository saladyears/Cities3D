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
#include "SpectatorListCtrl.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxSpectatorRestartListCtrl
// 
// Displays spectators in the <wxNetworkRestart> dialog.
//
// Derived From:
//     <wxSpectatorListCtrl>
//
// Project:
//     <Cities3D>
//
// Include:
//     SpectatorRestartListCtrl.h
//
class wxSpectatorRestartListCtrl : public wxSpectatorListCtrl
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxSpectatorRestartListCtrl
	// 
	// The wxSpectatorRestartListCtrl constructor.  Creates the columns for
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
	wxSpectatorRestartListCtrl(wxWindow* parent, wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize);

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
	// Sets the item color based on the spectator's replacement state.
	//
	// Parameters:
	//     index - The index to set.
	//
	virtual void SetSpectatorColor(const size_t index);
};

//---------------------------- PROTOTYPES       -----------------------------//

