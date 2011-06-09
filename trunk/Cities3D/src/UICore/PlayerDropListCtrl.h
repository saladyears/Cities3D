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
#include "PlayerListCtrl.h"
#include "DropSource.h"
#include "DropTarget.h"
#include "Player.h"

//---------------------------- DEFINES          -----------------------------//
namespace
{
	enum
	{
		ID_PLAYER_LIST				= 0x00000200,
	};
}

//---------------------------- TYPEDEFS         -----------------------------//
class wxListEvent;

class Game;
typedef boost::shared_ptr<Game> GamePtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxPlayerDropListCtrl
// 
// Displays all of the players in the <PlayerDatabase> in a list control,
// with drag and drop initiation.  Players are displayed as in the base
// class <wxPlayerListCtrl>.  However, with wxPlayerDropListCtrl, users
// can select a player in the list, hold down the left mouse button, and
// drag them to another object that supports receiving <Player> objects.  The
// class also implements a mechanism for users to directly add a player in
// the list to the current <Game> by handling the double-click mouse event
// for the list control.  If a player name is double-clicked, the player is
// added to the game via the RuleUIAddPlayer <Rule>.
//
// A wxPlayerDropListCtrl object also refreshes itself whenever the 
// eventPreGame <Event> is triggered.  It repopulates itself with every
// player in the <PlayerDatabase> and then removes all players from 
// the list that are already in the game.
//
// Derived From:
//     <wxPlayerListCtrl>
//     <DropSource>
//     <DropTarget>
//
// Project:
//     <Cities3D>
//
// Include:
//     PlayerDropListCtrl.h
//
class UI_EXPORT_SPEC wxPlayerDropListCtrl 
	: public wxPlayerListCtrl
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
	// Constructor: wxPlayerDropListCtrl
	// 
	// The wxPlayerDropListCtrl constructor.  Calls the base
	// <wxPlayerListCtrl> constructor, then adds OnUpdatePlayers
	// to the <Controller> as a receiver of the eventPreGame <Event>.
	//
	// Parameters:
	//     parent - The parent window.
	//     id - The message handling ID.  Should be a unique (to the parent
	//          window) ID, if the parent window wants to receive messages from
	//          the control.
	//     pos - The list control position in window coordinates.
	//     size - The list control size.
	//
	wxPlayerDropListCtrl(wxWindow* parent, wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxPlayerDropListCtrl
	// 
	// The wxPlayerDropListCtrl destructor.  Removes OnUpdatePlayers 
	// as a receiver of the eventPreGame <Event>.
	//
	~wxPlayerDropListCtrl();

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
	// Handles a drop event. Removes the player from the current game.
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
	// Handles a left mouse down event.  Checks to see if the mouse landed on a
	// player name.  If so, starts a drop operation with the selected <Player>.
	//
	virtual void LeftDown(wxMouseEvent &);

	//-----------------------------------------------------------------------//
	// Function: LeftUp
	//
	// Handles a left mouse up event.  Drops the selected <Player> if possible.
	//
	virtual void LeftUp(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnItemDoubleClick
	//
	// Handles a double-click in the player list.  If a player name is 
	// selected, the RuleUIAddPlayer <Rule> is executed and the <Player> data 
	// corresponding to the selected player name is added to the game.
	//
	virtual void LeftDoubleClick(wxMouseEvent &);

	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnUpdatePlayers
	//
	// Repopulates the player list, removing all players
	// already in the game.  Triggered by the eventPreGame
	// <Event>.
	//
	// Parameters:
	//     game - The current <Game>.
	//
	void OnUpdatePlayers(const GamePtr &game);

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

