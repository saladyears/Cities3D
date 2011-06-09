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
class DataObject;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxMessageUI
// 
// Displays game-related messages to the user. The wxMessageUI is the box 
// that sits in the upper-right corner of the UI and posts game-related
// information and messages to the user, telling them what is going on 
// at that time.  wxMessageUI is updated by transmitting a <DataObject>
// to the eventMessageUI <Event> via the <Controller>.  The transmitted
// <DataObject> must have a wxString as its first parameter and the
// current <Game> object as its second.  The wxMessageUI sets its output 
// text to the wxString and updates the color of text based on the color 
// of the current player, determined through the <Game> object.
//
// Derived From:
//     wxPanel
//
// Project:
//     <Cities3D>
//
// Include:
//     wxMessageUI.h
//
class wxMessageUI : public wxBaseUI<wxPanel>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxMessageUI
	//
	// The wxMessageUI constructor.  Makes the wxMessageUI visible
	// and adds OnUpdate as a receiver of the eventMessageUI <Event>.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//
	wxMessageUI(wxWindow *parent);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxMessageUI
	//
	// The wxMessageUI destructor.  Removes OnUpdate as a receiver
	// of the eventMessageUI <Event>.
	//
	~wxMessageUI();

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnUpdate
	//
	// Updates the text in the wxMessageUI.  Triggered by
	// the eventMessageUI <Event>.  The given <DataObject>
	// must have a wxString as its first parameter and the current
	// <Game> as its second.  The <wxBaseStaticText> control is
	// set to the wxString text and changes its color to the color
	// of the current player (or white if none).
	//
	// Parameters:
	//     object - The <DataObject> containing the update information.
	//              Must have a wxString as its first parameter and a 
	//              <Game> object as its second.
	//
	void OnUpdate(const DataObject &object);
};

//---------------------------- PROTOTYPES       -----------------------------//
