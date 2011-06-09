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
#include "BasePlayerGameListCtrl.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxPlayerGameListCtrl
// 
// Displays all of the players in a non-networked game.
// 
// Derived From:
//     <wxBasePlayerGameListCtrl>
//
// Project:
//     <Cities3D>
//
// Include:
//     PlayerGameListCtrl.h
//
class wxPlayerGameListCtrl : public wxBasePlayerGameListCtrl
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxPlayerGameListCtrl
	// 
	// The wxPlayerGameListCtrl constructor.
	//
	// Parameters:
	//     parent - The parent window.
	//     id - The message handling ID.  Should be a unique (to the parent
	//          window) ID, if the parent window wants to receive messages from
	//          the control.
	//     pos - The list control position in window coordinates.
	//     size - The list control size.
	//
	wxPlayerGameListCtrl(wxWindow* parent, wxWindowID id, 
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
	// Function: ChangeColor
	//
	// Changes the color of the given <Player> to the given color.
	//
	// Parameters:
	//     player - The <Player> to alter.
	//     color - The color to change to.
	//
	virtual void ChangeColor(const Player &player, const ColorType color);

	//-----------------------------------------------------------------------//
	// Function: RemovePlayer
	//
	// Removes the <Player> from the <Game>.
	//
	// Parameters:
	//     player - The <Player> to remove from the <Game>.
	//
	virtual void RemovePlayer(const Player &player);

	//-----------------------------------------------------------------------//
	// Function: AddPlayer
	//
	// Adds the <Player> to the <Game> with the given color.
	//
	// Parameters:
	//     player - The <Player> to add to the <Game>.
	//     color - The color to assign to the <Player>.
	//
	virtual void AddPlayer(const Player &player, const ColorType color);
};

//---------------------------- PROTOTYPES       -----------------------------//

