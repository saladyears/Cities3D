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
#include "DropSource.h"
#include "DropTarget.h"
#include "Player.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxSpectatorDropListCtrl
// 
// Displays all of the spectators in the current game and allows drag and drop
// operations with them.
//
// Derived From:
//     <wxSpectatorListCtrl>
//     <DropSource>
//     <DropTarget>
//
// Project:
//     <Cities3D>
//
// Include:
//     SpectatorListCtrl.h
//
class UI_EXPORT_SPEC wxSpectatorDropListCtrl 
	: public wxSpectatorListCtrl
	, public DropSource<Player>
	, public DropTarget<Player>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxSpectatorDropListCtrl
	// 
	// The wxSpectatorDropListCtrl constructor.
	//
	// Parameters:
	//     parent - The parent window.
	//     id - The message handling ID.  Should be a unique (to the parent
	//          window) ID, if the parent window wants to receive messages from
	//          the control.
	//     pos - The list control position in window coordinates.
	//     size - The list control size.
	//
	wxSpectatorDropListCtrl(wxWindow* parent, wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize);

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnDragOver
	//
	// Does nothing.
	//
	virtual bool OnDragOver(wxCoord, wxCoord, const Player&) 
	{
		return !InDrag();
	}

	//-----------------------------------------------------------------------//
	// Function: OnData
	//
	// Handles a drop event.  Occurs when the player has released the mouse 
	// button over the control with a valid <Player> data object to drop.  
	// Removes the player from the current game and adds them as a spectator.
	//
	// Parameters:
	//     player - The <Player> data.
	//
	virtual void OnData(wxCoord, wxCoord, const Player &player);

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: MouseMove
	//
	// Handles a mouse move event.  Drags the selected <Player> if possible.
	//
	virtual void MouseMove(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: LeftDown
	//
	// Handles a left mouse down event.  Checks to see if the mouse landed on 
	// a player name.  If so, sets the item to selected and then creates a new 
	// <wxPlayerDataObject> with the <Player> data for the selected player 
	// name. Finally, creates a wxDropSource object for doing drag
	// and drop in wxWidgets.
	//
	virtual void LeftDown(wxMouseEvent &);

	//-----------------------------------------------------------------------//
	// Function: LeftUp
	//
	// Handles a left mouse up event.  Drops the selected <Player> if possible.
	//
	virtual void LeftUp(wxMouseEvent &event);
	
	//-----------------------------------------------------------------------//
	// Function: LeftDoubleClick
	//
	// Handles a left double-click event.  Attempts to add the player to the
	// game at the first available color, if possible.
	//
	void LeftDoubleClick(wxMouseEvent &);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Int: mDragIndex
	//
	// The index of any item being dragged within the list.
	//
	wxInt32 mDragIndex;
};

//---------------------------- PROTOTYPES       -----------------------------//

