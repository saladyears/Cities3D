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
// Class: wxGameListCtrl
// 
// A list control displaying all games in the <GameDatabase>.  
//
// Derived From:
//     <wxBaseListCtrl>
//
// Project:
//     <Cities3D>
//
// Include:
//     GameListCtrl.h
//
class wxGameListCtrl : public wxBaseListCtrl
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxGameListCtrl
	// 
	// The wxGameListCtrl constructor.  Creates the columns for
	// the list control and then calls ImportGames to populate
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
	wxGameListCtrl(wxWindow* parent, wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize);

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: ImportGames
	//
	// Reads all the maps from the <GameDatabase> and inserts
	// the information into the list control.
	//
	void ImportGames();
};

//---------------------------- PROTOTYPES       -----------------------------//
