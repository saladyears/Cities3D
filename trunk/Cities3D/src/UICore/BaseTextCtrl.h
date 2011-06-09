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
#include "BaseSkinMixin.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxBaseTextCtrl
// 
// A basic text control.  A text control is a UI mechanism for
// users to enter text.  The class wxBaseTextCtrl provides a simple
// base class for all text controls in the game, providing a 
// common look and feel across UI objects.  It is in all ways like
// a wxTextCtrl, except that it sets its background to the background 
// color of the parent window.
//
// Derived From:
//     wxTextCtrl (see <http://www.wxwidgets.org/manuals/2.4.2/wx381.htm>),
//     <BaseSkinMixin>
//
// Project:
//     <UICore>
//
// Include:
//     BaseTextCtrl.h
//
class UI_EXPORT_SPEC wxBaseTextCtrl 
	: public wxTextCtrl
	, public BaseSkinMixin<wxBaseTextCtrl>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxBaseTextCtrl
	//
	// The wxBaseTextCtrl constructor.  Creates the text control with the
	// given position, style, size, etc.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//     id - The message handling ID.  Should be a unique (to the parent
	//          window) ID, if the parent window wants to receive messages from
	//          the control.
	//     pos - The button position in window coordinates.
	//     size - The button size.
	//     style - The button style.
	//
	wxBaseTextCtrl(wxWindow* parent, wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, long style = 0,
		const HashString &color = shDefaultText,
		const HashString &font = shDefaultFont);

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
	// Function: OnTextEnter
	//
	// Handles the user pressing the Enter key while in the text box.
	//
	// Parameters:
	//     event - The Enter event.
	//
	void OnTextEnter(wxCommandEvent&);
};

//---------------------------- PROTOTYPES       -----------------------------//
