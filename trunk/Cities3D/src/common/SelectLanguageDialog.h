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
// Class: wxSelectLanguageDialog
// 
// Dialog that lets the user select their default language.
//
// Derived From:
//     <wxBaseModalDialog>
//
// Project:
//     <Cities3D>
//
// Include:
//     SelectLanguageDialog.h
//
class wxSelectLanguageDialog : public wxBaseModalDialog
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxSelectLanguageDialog
	//
	// The wxSelectLanguageDialog constructor.
	//
	// Parameters:
	//     parent - The parent window.  May be NULL.
	//
	wxSelectLanguageDialog(wxWindow *parent);

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
	// Handles the OK event.  Saves the user's selected language for later use.
	//
	virtual void Ok();

	//-----------------------------------------------------------------------//
	// Function: Cancel
	//
	// Disables the Cancel event.  The user must make a selection.
	//
	virtual void Cancel() {}
};

//---------------------------- PROTOTYPES       -----------------------------//

