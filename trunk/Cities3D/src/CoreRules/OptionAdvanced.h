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
#include "OptionWindow.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxOptionAdvanced
// 
// A <wxSettingsDialog> option window that encapsulates  UI needed for 
// advanced OpenGL adjustments.
//
// Derived From:
//     <wxOptionWindow>
//
// Project:
//     <Cities3D>
//
// Include:
//     OptionDisplaySize.h
//
class wxOptionAdvanced : public wxOptionWindow
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxOptionAdvanced
	//
	// The wxOptionAdvanced constructor.  Lays out the UI elements needed by 
	// the option window.
	//
	// Parameters:
	//     parent - The parent window.
	//
	wxOptionAdvanced(wxWindow* parent);

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
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnRender
	//
	// Handles the framebuffer checkbox being changed.
	//
	// Parameters:
	//     event - The command event.
	//
	void OnRender(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnFramebuffer
	//
	// Handles the framebuffer checkbox being changed.
	//
	// Parameters:
	//     event - The command event.
	//
	void OnFramebuffer(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnPixelbuffer
	//
	// Handles the pixelbuffer checkbox being changed.
	//
	// Parameters:
	//     event - The command event.
	//
	void OnPixelbuffer(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Function: On16Radio
	//
	// Handles the 16 bit radio button being clicked.
	//
	// Parameters:
	//     event - The command event.
	//
	void On16Radio(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Function: On24Radio
	//
	// Handles the 24 bit radio button being clicked.
	//
	// Parameters:
	//     event - The command event.
	//
	void On24Radio(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Function: On32Radio
	//
	// Handles the 32 bit radio button being clicked.
	//
	// Parameters:
	//     event - The command event.
	//
	void On32Radio(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnDebug
	//
	// Handles the debug checkbox being changed.
	//
	// Parameters:
	//     event - The command event.
	//
	void OnDebug(wxCommandEvent &event);
};

//---------------------------- PROTOTYPES       -----------------------------//
