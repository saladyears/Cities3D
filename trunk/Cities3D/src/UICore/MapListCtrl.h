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
// Class: wxMapListCtrl
// 
// A list control displaying all maps in the <MapDatabase>.  Each
// map in the list is displayed with the map name, number of players the
// map is for, the default points to play to, and the name of the 
// map creator.
//
// Derived From:
//     <wxBaseListCtrl>
//
// Project:
//     <Cities3D>
//
// Include:
//     MapListCtrl.h
//
class UI_EXPORT_SPEC wxMapListCtrl : public wxBaseListCtrl
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxMapListCtrl
	// 
	// The wxMapListCtrl constructor.  Creates the columns for
	// the list control and then calls ImportMaps to populate
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
	wxMapListCtrl(wxWindow* parent, wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize);

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: SortItems
	// 
	// Sorts the items by the given column index.  If
	// the column is immediately clicked again, the sort order is
	// reversed.
	//
	// Parameters:
	//     col - The column index to sort by.
	//
	void SortItems(wxInt32 col);

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: ImportMaps
	//
	// Reads all the maps from the <MapDatabase> and inserts
	// the information into the list control.
	//
	void ImportMaps();
};

//---------------------------- PROTOTYPES       -----------------------------//
