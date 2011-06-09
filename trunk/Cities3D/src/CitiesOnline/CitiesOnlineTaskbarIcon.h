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
#include <wx/taskbar.h> 

//---------------------------- USER INCLUDES    -----------------------------//

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxCitiesOnlineTaskbarIcon
// 
// Implements the taskbar icon functionality for the applicaton.
//
// Derived From:
//     <wxTaskBarIcon>
//
// Project:
//     <CitiesOnline>
//
// Include:
//     CitiesOnlineTaskbarIcon.h
//
class wxCitiesOnlineTaskbarIcon : public wxTaskBarIcon
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxCitiesOnlineTaskbarIcon
	//
	// The default wxCitiesOnlineTaskbarIcon constructor.
	//
	wxCitiesOnlineTaskbarIcon();

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: CreatePopupMenu
	//
	// Creates the CitiesOnline specific popup menu in the tray.
	//
	// Returns:
	//   The popup menu.
	//
	virtual wxMenu* CreatePopupMenu(); 

	//-----------------------------------------------------------------------//
	// Function: GetUrlMenuId
	//
	// Returns the id associated with the fake url handling menu item.
	//
	// Returns:
	//     An int that is the id of the menu item.
	//
	wxInt32 GetUrlMenuId() const;

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
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnMenuExit
	//
	// Handles the Exit click.
	//
	void OnMenuExit(wxCommandEvent&);

	//-----------------------------------------------------------------------//
	// Function: OnMenuUrl
	//
	// Handles the fake url message from the application.
	//
	void OnMenuUrl(wxCommandEvent&);
};

//---------------------------- PROTOTYPES       -----------------------------//

