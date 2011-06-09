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
#include "PaneMixin.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxStatsPane
// 
// Shows the game stats.
//
// Derived From:
//     wxPanel
//     <PaneMixin>
//
// Project:
//     <CitiesOnline>
//
// Include:
//     SettingsPane.h
//
class wxStatsPane 
	: public wxPanel
	, public PaneMixin
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxStatsPane
	//
	// The wxStatsPane constructor. 
	//
	// Parameters:
	//     parent - The parent window.
	//     id - The id of the menu controlling this pane.
	//
	wxStatsPane(wxWindow *parent, wxInt32 id);

	//-----------------------------------------------------------------------//
	// Group: Destructors
	//
	
	//-----------------------------------------------------------------------//
	// Destructor: ~wxStatsPane
	//
	// The wxStatsPane destructor.
	//
	~wxStatsPane();


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
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnStats
	//
	// Refreshes the stats display.
	//
    // Parameters:
    //     id - The id of the game we're showing stats for.
    //
	void OnStats(wxInt32 id);
};

//---------------------------- PROTOTYPES       -----------------------------//
