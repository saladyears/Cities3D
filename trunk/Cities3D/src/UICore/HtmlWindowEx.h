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
#include <wx/html/htmlwin.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "UIExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxHtmlWindowEx
// 
// Prevents mouse wheel messages from propagating.
//
// Derived From:
//     <wxHtmlWindow>
//
// Project:
//     <Cities3D>
//
// Include:
//     HtmlWindowEx.h
//
class UI_EXPORT_SPEC wxHtmlWindowEx : public wxHtmlWindow
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxHtmlWindowEx
	//
	// The wxHtmlWindowEx constructor.
	//
	// Parameters:
	//     parent - The parent window.
	//     id - The message handling ID.
	//     pos - The text control position in window coordinates.
	//     size - The text control size.
	//
	wxHtmlWindowEx(wxWindow* parent, wxInt32 id, const wxPoint& pos = 
		wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0) 
		: wxHtmlWindow(parent, id, pos, size, wxHW_DEFAULT_STYLE | 
		wxHW_NO_SELECTION | style) {}

	//-----------------------------------------------------------------------//
	// Group: Functions
	//
	
	//-----------------------------------------------------------------------//
	// Function: AddText
	//
	// Adds the given string to the window.
	//
	// Parameters:
	//     text - The string to add.
	//
	void AddText(const wxString& text);

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
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: ScrollNice
	//
	// Keeps the window nicely scrolled.
	//
	void ScrollNice();

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnMouseWheel
	//
	// Handles mouse wheel events.
	//
	// Parameters:
	//     event - The event to handle.
	//
	void OnMouseWheel(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnKeyDown
	//
	// Handles keys being pressed.
	//
	// Parameters:
	//     event - The key event.
	//
	void OnKeyDown(wxKeyEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnSize
	//
	// Handles new sizes.
	//
	// Parameters:
	//     event - The key event.
	//
	void OnSize(wxSizeEvent &event);
};

//---------------------------- PROTOTYPES       -----------------------------//

