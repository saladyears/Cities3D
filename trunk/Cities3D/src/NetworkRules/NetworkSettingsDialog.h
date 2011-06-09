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

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxNetworkSettingsDialog
// 
// Dialog that lets the user select a port for their server
// to listen on in a direct connect network game.
//
// Derived From:
//     <wxBaseModalDialog>
//
// Project:
//     <Cities3D>
//
// Include:
//     NetworkSettingsDialog.h
//
class wxNetworkSettingsDialog : public wxBaseModalDialog
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxNetworkSettingsDialog
	//
	// The wxNetworkSettingsDialog constructor.
	//
	// Parameters:
	//     parent - The parent window.
	//
	wxNetworkSettingsDialog(wxWindow *parent);

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: GetAddress
	//
	// Returns the hostname and port information entered by the user.
	//
	// Returns:
	//     A const reference to a wxIPV4address containing the hostname and 
	//     port information.
	//
	const wxIPV4address &GetAddress() const {return mAddress;}

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
	// Handles the OK event.  Saves the port information
	// for later use.
	//
	virtual void Ok();

	//-----------------------------------------------------------------------//
	// Function: mAddress
	//
	// The user's hostname and port.
	//
	wxIPV4address mAddress;
};

//---------------------------- PROTOTYPES       -----------------------------//

