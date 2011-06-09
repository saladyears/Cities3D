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
#include <wx/listctrl.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "BaseModalDialog.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxSelectMapDialog
// 
// Dialog that allows map selection and Cities & Knights
// rules to be turned on.
//
// Derived From:
//     <wxBaseModalDialog>
//
// Project:
//     <Cities3D>
//
// Include:
//     SelectMapDialog.h
//
class UI_EXPORT_SPEC wxSelectMapDialog : public wxBaseModalDialog
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxSelectMapDialog
	//
	// The wxSelectMapDialog constructor.  If a map
	// is currently loaded, selects the map in the 
	// <wxMapTextCtrl>.  Sets the Cities & Knights
	// <wxBaseCheckBox> to the given value. Disables
	// the OK button.
	//
	// Parameters:
	//     parent - The parent window.
	//     index - The index of the current selected map.  May
	//             be -1 if no map is selected.
	//
	wxSelectMapDialog(wxWindow *parent, wxInt32 index);
	
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: GetIndex
	//
	// Returns the <MapDatabase> index of the selected map.
	//
	// Returns:
	//     The <MapDatabase> index of the selected map.
	//
	wxInt32 GetIndex() const {return mIndex;}

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
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Ok
	//
	// Handles the OK button click event.  Updates the mIndex flag and then 
	// calls the base class Ok handler to exit the dialog.
	//
	virtual void Ok();

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnMapSelected
	//
	// Handles a selection in the <wxMapTextCtrl>.
	// Enables the OK button if a map is selected, disables
	// it if not.
	//
	// Parameters:
	//     event - The list selection event.
	//
	void OnMapSelected(wxListEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnMapDoubleClick
	//
	// Handles a double-click in the <wxMapTextCtrl>.
	// Immediately calls Ok to exit the dialog.  Shortcut
	// instead of having to select a map and click the 
	// OK button.
	//
	// Parameters:
	//     event - The list selection event.
	//
	void OnMapDoubleClick(wxListEvent &event);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Int: mIndex
	//
	// The index in the <MapDatabase> of the selected map.
	//
	wxInt32 mIndex;
};

//---------------------------- PROTOTYPES       -----------------------------//

