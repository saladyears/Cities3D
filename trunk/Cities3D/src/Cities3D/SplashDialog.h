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

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxSplashDialog
// 
// Dialog that displays an initial splash window for the application
// for a brief time period.
//
// Derived From:
//     wxDialog
//
// Project:
//     <Cities3D>
//
// Include:
//     SplashDialog.h
//
class wxSplashDialog : public wxDialog
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Create
	//
	// Creates the wxSplashDialog.  Loads the splash image from the image
	// engine.
	//
	// Parameters:
	//     parent - The parent window.
	//
	void Create(wxWindow *parent);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxSplashDialog
	//
	// The wxSplashDialog destructor.  Clears the splash image memory.
	//
	~wxSplashDialog();

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnFindPlugin
	//
	// Updates the text with the plugin discovered.
	//
	// Parameters:
	//     str - The string text.
	//
	void OnFindPlugin(const wxString& str);
};

//---------------------------- PROTOTYPES       -----------------------------//

