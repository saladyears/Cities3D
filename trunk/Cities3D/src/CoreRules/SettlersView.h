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
#include <list>

//---------------------------- USER INCLUDES    -----------------------------//
#include "SettlersGLCanvas.h"
#include "IPopup.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class DataObject;
class wxAuiManager;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxSettlersView
// 
// The main game window.  wxSettlersView houses all of the <UI> elements in the
// game, maintaining them when the size of the main window changes, or when a 
// user initiates a total UI resize event.  wxSettlersView is built on top of 
// <wxSettlersGLCanvas>, which means that all of the UI and OpenGL display 
// comes together in one place, cleanly separated through inheritance.
//
// Derived From:
//     <wxSettlersGLCanvas>
//
// Project:
//     <Cities3D>
//
// Include:
//     wxSettlersView.h
//
class wxSettlersView 
	: public wxSettlersGLCanvas
	, public IPopup
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxSettlersView
	// 
	// The wxSettlersView constructor.
	//
	// Parameters:
	//     parent - The parent window (must not be NULL).
	//     manager - The wxAuiManager responsible for managing the UI.
	//     id - The id for message handling.
	//     pos - The starting position.
	//     size - The starting size.
	//     style - The window style, using wxWindow parameters.
	//     name - The name.
	//     attrib - Starting OpenGL implementation attributes.
	//
	wxSettlersView(wxWindow *parent, wxAuiManager* manager,
		const wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0, 
		const wxString& name = swWxSettlersView, wxInt32 *attrib = NULL);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxSettlersView
	//
	// The wxSettlersView destructor.  
	//
	~wxSettlersView();

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Type
	//
	// Returns the type of popup this is.
	//
	// Returns:
	//     The type of popup this is.
	//
	PopupType Type() const
	{
		return PopupRequestOnly;
	}

	//-----------------------------------------------------------------------//
	// Function: TestPopup
	//
	// Tests whether the given mouse point (in screen coords) is on a window
	// that can display a popup.
	//
	// Parameters:
	//     pos - The mouse position to test, in screen coords.
	//     delay - [out] The number of ticks of delay before this popup should
	//             be displayed.
	//
	// Returns:
	//     A bool that is true if the coords are over a popup, and false if
	//     not.
	//
	virtual bool TestPopup(const wxPoint& pos, wxInt32& delay) const;

	//-----------------------------------------------------------------------//
	// Function: CreatePopup
	//
	// Populates the given sizer to create the popup.
	//
	// Parameters:
	//     window - The parent popup window.
	//     sizer - The sizer to populate.
	//
	virtual void CreatePopup(wxWindow* window, wxSizer* sizer) const;

	//-----------------------------------------------------------------------//
	// Function: GetThis
	//
	// Returns the window the popup is attached to.
	//
	// Returns:
	//     The window the popup is attached to.
	//
	virtual wxWindow* GetThis() const;

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Function: GenerateDebugInfo
	//
	// Displays debug info to the screen.
	// 
	// Parameters:
	//     str - The string of debug info.
	//
	virtual void GenerateDebugInfo(StringArray &str);

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Pointer: mAuiManager
	//
	// The AUI manager responsible for managing game windows.
	//
	wxAuiManager* mAuiManager;
};

//---------------------------- PROTOTYPES       -----------------------------//

