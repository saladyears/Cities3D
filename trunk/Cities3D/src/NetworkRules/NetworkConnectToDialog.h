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
#include "BaseModalDialog.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxNetworkConnectToDialog
// 
// Dialog that lets the user select an IP and port to connect
// to in a direct connect network game.
//
// Derived From:
//     <wxBaseModalDialog>
//
// Project:
//     <Cities3D>
//
// Include:
//     NetworkConnectToDialog.h
//
class wxNetworkConnectToDialog : public wxBaseModalDialog
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxNetworkConnectToDialog
	//
	// The wxNetworkConnectToDialog constructor.
	//
	// Parameters:
	//     parent - The parent window.
	//
	wxNetworkConnectToDialog(wxWindow *parent);

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: GetAddress
	//
	// Returns the address information entered by the user.
	//
	// Returns:
	//     A reference to the address information entered
	//     by the user.
	//
	const wxIPV4address &GetAddress() const
	{
		return mAddress;
	}

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Ok
	//
	// Handles the OK event.  Saves the address/port information
	// for later use.
	//
	virtual void Ok();

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Object: mAddress
	//
	// The IP address and port selected by the user.
	//
	wxIPV4address mAddress;
};

//---------------------------- PROTOTYPES       -----------------------------//

