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
#include "UIExport.h"
#include "BaseListCtrl.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxContextListCtrl
//
// A list control designed to work in a context menu.
//
// Derived From:
//     <wxBaseListCtrl>
//
// Project:
//     <UICore>
//
// Include:
//     ContextListCtrl.h
//
class UI_EXPORT_SPEC wxContextListCtrl : public wxBaseListCtrl
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxContextListCtrl
	//
	// The wxContextList constructor.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//
	wxContextListCtrl(wxWindow *parent);

	//-----------------------------------------------------------------------//
	// Group: Functions
	//
	
	//-----------------------------------------------------------------------//
	// Function: AddMenuItem
	//
	// Adds an item as a selection to the list control.
	//
	// Parameters:
	//     text - The selection text.
	//     id - The selection id.
	//     icon - The selection icon.
	//
	void AddMenuItem(const wxString &text, const wxInt32 id, 
		const HashString &icon);

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//
	
	//-----------------------------------------------------------------------//
	// Function: LeftDown
	//
	// Handles the left down event by sending out a button click message for
	// the selected item.
	//
	// Parameters:
	//     event - The mouse event.
	//
	virtual void LeftDown(wxMouseEvent &event);
};

//---------------------------- PROTOTYPES       -----------------------------//

