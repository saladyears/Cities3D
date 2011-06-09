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
#include "BaseTextCtrl.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxChatInputTextCtrl
// 
// Derived From:
//     <wxBaseTextCtrl>
//
// Project:
//     <Cities3D>
//
// Include:
//     ChatInputTextCtrl.h
//
class UI_EXPORT_SPEC wxChatInputTextCtrl : public wxBaseTextCtrl
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxChatInputTextCtrl
	//
	// The wxChatInputTextCtrl constructor.  Creates the text control with the
	// given position, style, size, etc.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//     id - The message handling ID.  Should be a unique (to the parent
	//          window) ID, if the parent window wants to receive messages from
	//          the control.
	//     pos - The button position in window coordinates.
	//     size - The button size.
	//
	wxChatInputTextCtrl(wxWindow* parent, wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxChatInputTextCtrl
	//
	// The wxChatInputTextCtrl destructor.
	//
	virtual ~wxChatInputTextCtrl();

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

    void KeyDown(wxKeyEvent& event);

	//-----------------------------------------------------------------------//
	// Function: OnKeyDown
	//
	// Adds characters to the control if the control has the focus.
	//
	// Parameters:
	//     event - The key press event.
	//
	void OnKeyDown(const wxKeyEvent &event);
};

//---------------------------- PROTOTYPES       -----------------------------//

