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
#include "BaseUI.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxEventUI
// 
// Houses the <wxEventWindow> during network games.  Just a thin wrapper that
// registers and unregisters the UI during game play.
//
// Derived From:
//     <wxBaseUI>
//
// Project:
//     <Cities3D>
//
// Include:
//     EventUI.h
//
class wxEventUI : public wxBaseUI<wxPanel>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxEventUI
	//
	// The wxEventUI constructor.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//
	wxEventUI(wxWindow *parent);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxEventUI
	//
	// The wxEventUI destructor.
	//
	~wxEventUI();

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnTabEvents
	//
	// Handles a raise tab event.
	//
	void OnTabEvents(wxInt32);
};

//---------------------------- PROTOTYPES       -----------------------------//

