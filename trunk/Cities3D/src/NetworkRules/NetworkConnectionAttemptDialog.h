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
#include "BaseModalDialog.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class INetworkClient;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxNetworkConnectionAttemptDialog
// 
// Dialog that gives the user feedback on how the attempt to connect to a game
// is going.
//
// Derived From:
//     <wxBaseModalDialog>
//
// Project:
//     <NetworkRules>
//
// Include:
//     NetworkConnectionAttemptDialog.h
//
class wxNetworkConnectionAttemptDialog : public wxBaseModalDialog
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxNetworkConnectionAttemptDialog
	//
	// The wxNetworkConnectionAttemptDialog constructor.
	//
	// Parameters:
	//     parent - The parent window.
	//     address - The ip address we are attempting to connect to.
	//
	wxNetworkConnectionAttemptDialog(wxWindow *parent, 
		const wxIPV4address& address, INetworkClient* client);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxNetworkConnectionAttemptDialog
	//
	// The wxNetworkConnectionAttemptDialog destructor.
	//
	~wxNetworkConnectionAttemptDialog();

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnNetworkConnectionEvent
	//
	// Handles a connection event.
	//
	// Parameters:
	//   str - The event string.
	//
	void OnNetworkConnectionEvent(const wxString& str);

	//-----------------------------------------------------------------------//
	// Group: Timer Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnTimer
	//
	// Allows the dialog to become modal before attempting to connect.
	//
	void OnTimer();

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Pointer: mpNetworkClient
	//
	// The network client interface we use to connect.
	//
	INetworkClient* mpNetworkClient;

	//-----------------------------------------------------------------------//
	// Object: mAddress
	//
	// The ip address to connect to.
	//
	wxIPV4address mAddress;
};

//---------------------------- PROTOTYPES       -----------------------------//

