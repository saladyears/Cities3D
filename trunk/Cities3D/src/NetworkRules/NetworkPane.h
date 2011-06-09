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
#include <wx/aui/aui.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "PaneMixin.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class DataObject;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxNetworkPane
// 
// Window pane that handles all direct connect network game setup.
//
// Derived From:
//     wxAuiNotebook
//     <IPaneMixin>
//
// Project:
//     <NetworkRules>
//
// Include:
//     NetworkPane.h
//
class wxNetworkPane 
	: public wxAuiNotebook
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
	// Constructor: wxNetworkPane
	//
	// The wxNetworkPane constructor.
	//
	// Parameters:
	//     parent - The parent window.
	//
	wxNetworkPane(wxWindow *parent);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxNetworkPane
	//
	// The wxNetworkPane destructor.
	//
	~wxNetworkPane();

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: ClosePane
	//
	// Handles cleanup when the user closes the pane manually.
	//
	virtual void ClosePane();

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnGameConnect
	//
	// Updates the pages when the connection status changes.
	//
	// Parameters:
	//     object - Data parameter.
	//
	void OnGameConnect(const DataObject& object);

	//-----------------------------------------------------------------------//
	// Function: OnPrepareNetworkGame
	//
	// Handles the user starting or joining a network game.
	//
	// Parameters:
	//     val - A flag that is true if the user is starting their own game
	//           and false if joining a game in progress.
	//
	void OnPrepareNetworkGame(bool own);
};

//---------------------------- PROTOTYPES       -----------------------------//

