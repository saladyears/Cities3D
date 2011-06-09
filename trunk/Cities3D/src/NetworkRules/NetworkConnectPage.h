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
#include <wx/socket.h>

//---------------------------- USER INCLUDES    -----------------------------//

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxListEvent;
class DataObject;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxNetworkConnectPage
// 
// Handles all network direct connect setup within the <wxNetworkNotebook> on 
// the <wxNetworkDialog>, including being a host or client, IP addresses, 
// network ports, and player selection.
//
// Derived From:
//     wxPanel
//
// Project:
//     <Cities3D>
//
// Include:
//     NetworkConnectPage.h
//
class wxNetworkConnectPage : public wxPanel
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxNetworkConnectPage
	//
	// The wxNetworkConnectPage constructor.  Creates all UI
	// elements and lays out the page.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//
	wxNetworkConnectPage(wxWindow* parent);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxNetworkConnectPage
	//
	// The wxNetworkConnectPage destructor.  
	//
	~wxNetworkConnectPage();

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
	// Function: OnGameConnect
	//
	// Updates the page when the connection status changes.
	//
	// Parameters:
	//     object - Data parameter.
	//
	void OnGameConnect(const DataObject& object);

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnHostRadio
	//
	// Handles a click on the Host radio button.  Allows
	// the user to start a server.
	//
	// Parameters:
	//     event - The button click event.
	//
	void OnHostRadio(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnClientRadio
	//
	// Handles a click on the Host radio button.  Allows
	// the user to connect to a server.
	//
	// Parameters:
	//     event - The button click event.
	//
	void OnClientRadio(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnActionButton
	//
	// Handles a click on the action button.  Either starts or stops a
	// server, or connects to or disconnects from a server.
	//
	// Parameters:
	//     event - The button click event.
	//
	void OnActionButton(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnSettingsButton
	//
	// Handles a click on the settings button.  Runs the
	// <wxNetworkSettingsDialog> dialog.
	//
	// Parameters:
	//     event - The button click event.
	//
	void OnSettingsButton(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnAddPlayer
	//
	// Runs the <wxAddPlayerDialog> window.  If the
	// user adds a new player, adds that player
	// to the <wxPlayerListCtrl>.
	//
	// Parameters:
	//     event - The button click event.
	//
	void OnAddPlayer(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnDeletePlayer
	//
	// Deletes the selected player in the <wxPlayerListCtrl>
	// from the <PlayerDatabase>.
	//
	// Parameters:
	//     event - The button click event.
	//
	void OnDeletePlayer(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnPlayerSelected
	//
	// Handles a player selection in the <wxPlayerListCtrl>.
	// Enables the Edit and Delete buttons if a player is selected,
	// disables them if not.
	//
	// Parameters:
	//     event - The list selection event.
	//
	void OnPlayerSelected(wxListEvent &event);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Object: mAddress
	//
	// The IP address and port the player is connecting to.
	//
	wxIPV4address mAddress;
};

//---------------------------- PROTOTYPES       -----------------------------//

