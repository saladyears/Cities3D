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
#include "DropSource.h"
#include "DropTarget.h"
#include "ColorInfoData.h"
#include "Player.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class PlayerGame;

class Game;
typedef boost::shared_ptr<Game> GamePtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxBasePlayerGameListCtrl
//
// Displays all of the players in the current <Game> in a list control,
// with full drag and drop support.  All of the colors in the system are
// displayed as list selections.  If a player is in the game, the color that
// they are assigned to has their player name next to it in the list.  If a
// selection in the list does not have a player name next to it, that color is
// available for any player to select.
//
// The class wxBasePlayerGameListCtrl is a drop target for any drag and drop 
// operation that uses <Player> objects.  The drop targets are the color nodes 
// themselves, so that a user's color may be directly selected with a 
// drag/drop.  Not only that, but wxBasePlayerGameListCtrl is also drag 
// initiator for itself, meaning that a user may alter the color of a player in
// the game by selecting the player with the left mouse button, holding down 
// the mouse, dragging the player to a new color, and then releasing the left 
// mouse button.
//
// Derived From:
//     <wxBaseListCtrl>
//     <DropSource>
//     <DropTarget>
//
// Project:
//     <Cities3D>
//
// Include:
//     PlayerGameListCtrl.h
//
class UI_EXPORT_SPEC wxBasePlayerGameListCtrl 
	: public wxBaseListCtrl
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
	// Constructor: wxBasePlayerGameListCtrl
	// 
	// The wxBasePlayerGameListCtrl constructor.  Adds all of the colors in
	// the system as selections to the control.
	//
	// Parameters:
	//     parent - The parent window.
	//     id - The message handling ID.  Should be a unique (to the parent
	//          window) ID, if the parent window wants to receive messages from
	//          the control.
	//     pos - The list control position in window coordinates.
	//     size - The list control size.
	//
	wxBasePlayerGameListCtrl(wxWindow* parent, wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxBasePlayerGameListCtrl
	//
	// The wxBasePlayerGameListCtrl destructor.
	//
	virtual ~wxBasePlayerGameListCtrl();

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnDragOver
	//
	// Handles the drag event when the mouse is over the control.  Does a hit 
	// test with the given coordinates and selects any color selection that 
	// may be underneath the potential drop position.
	//
	// Parameters:
	//     x - The x coordinate of the drag position.
	//     y - The y coordinate of the drag position.
	//     data - The data that may be dropped.
	//
	// Returns:
	//     A bool that is true if the drag operation is supported by the 
	//     target and false if not.
	//
	virtual bool OnDragOver(wxCoord x, wxCoord y, const Player& data);

	//-----------------------------------------------------------------------//
	// Function: OnData
	//
	// Handles a drop event.  Occurs when the player has released the mouse 
	// button over the control with a valid <Player> data object to drop.  First 
	// checks to ensure that the drop position is valid and then does a 
	// combination of three things.  Adds the player to the  game if the player
	// is not already in the tree, and/or changes the color  of the player if 
	// they were already in the tree, and/or removes a player from the game if 
	// they were already at the dropped color.
	//
	// Parameters:
	//     x - The x coordinate of the drop position.
	//     y - The y coordinate of the drop position.
	//     player - The <Player> data.
	//
	virtual void OnData(wxCoord x, wxCoord y, const Player &player);

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//
	
	//-----------------------------------------------------------------------//
	// Function: ChangeColor
	//
	// Changes the color of the given <Player> to the given color.
	//
	// Parameters:
	//     player - The <Player> to alter.
	//     color - The color to change to.
	//
	virtual void ChangeColor(const Player &player, const ColorType color)=0;

	//-----------------------------------------------------------------------//
	// Function: RemovePlayer
	//
	// Removes the <Player> from the <Game>.
	//
	// Parameters:
	//     player - The <Player> to remove from the <Game>.
	//
	virtual void RemovePlayer(const Player &player)=0;

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
	// color selection with a player name.   If so, creates a new 
	// <wxPlayerDataObject> with the <Player> data for  the selected player 
	// name. Finally, creates a wxDropSource object for doing drag and drop in 
	// wxWidgets.
	//
	// Parameters:
	//      event - The mouse event.
	//
	virtual void LeftDown(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: LeftUp
	//
	// Handles a left mouse up event.  Drops the selected <Player> if possible.
	//
	virtual void LeftUp(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: LeftDoubleClick
	//
	// Handles a left double-click event.  If a player name is selected, 
	// the RemovePlayer function is called.
	//
	virtual void LeftDoubleClick(wxMouseEvent &);

	//-----------------------------------------------------------------------//
	// Function: SetItemColour
	//
	// Does nothing.
	//
	virtual void SetItemColor(const PlayerGame &, const wxInt32) {}

	//-----------------------------------------------------------------------//
	// Function: AddPlayer
	//
	// Adds the <Player> to the <Game> with the given color.
	//
	// Parameters:
	//     player - The <Player> to add to the <Game>.
	//     color - The color to assign to the <Player>.
	//
	virtual void AddPlayer(const Player &player, const ColorType color)=0;

	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnUpdatePlayers
	//
	// Sets player names for all <Players> in the <Game> at the selection with
	// their color.  Triggered by the eventPreGame <Event>.
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

