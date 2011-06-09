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

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxBaseFrame
// 
// A basic frame window.
//
// Derived From:
//     wxFrame
//
// Project:
//     <UICore>
//
// Include:
//     BaseFrame.h
//
class UI_EXPORT_SPEC wxBaseFrame : public wxFrame
{
	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxBaseFrame
	//
	// The wxBaseFrame constructor.
	//
	wxBaseFrame(wxWindow *parent, const wxString &caption, 
		const wxPoint &pos = wxDefaultPosition, 
		const wxSize &size = wxDefaultSize);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxBaseFrame
	//
	// The wxBaseFrame destructor
	//
	virtual ~wxBaseFrame();

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Size
	//
	// Recalculates button location.
	//
	virtual void Size(wxSizeEvent &)
	{
	}

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
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnSkin
	//
	// Updates the buttons on a skin change.
	//
	virtual void OnSkin(wxInt32 = 0);

	//-----------------------------------------------------------------------//
	// Function: OnToggleFullscreen
	//
	// Handles fullscreen mode being toggled.
	//
	void OnToggleFullscreen(wxInt32 = 0);

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnSize
	//
	// Handles size changes.
	//
	// Parameters:
	//     event - The size event.
	//
	void OnSize(wxSizeEvent &event);
};

//---------------------------- PROTOTYPES       -----------------------------//
