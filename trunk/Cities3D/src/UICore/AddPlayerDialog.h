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
class Player;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxAddPlayerDialog
// 
// Dialog that allows users to add or edit players.
//
// Derived From:
//     <wxBaseModalDialog>
//
// Project:
//     <Cities3D>
//
// Include:
//     AddPlayerDialog.h
//
class UI_EXPORT_SPEC wxAddPlayerDialog : public wxBaseModalDialog
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxAddPlayerDialog
	//
	// The wxAddPlayerDialog constructor.
	//
	// Parameters:
	//     parent - The parent window.
	//
	wxAddPlayerDialog(wxWindow *parent);

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: GetPlayer
	//
	// Returns the local <Player> data being edited.
	//
	// Returns:
	//     A const reference to the local <Player> data.
	//
	const Player &GetPlayer();

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
	// Handles the OK button click event.  Updates the local <Player> data 
	// with all information from the dialog window.  Adds the <Player> to the 
	// <PlayerDatabase>.  Then calls the base class Ok handler to exit the 
	// dialog.
	//
	virtual void Ok();
};

//---------------------------- PROTOTYPES       -----------------------------//

