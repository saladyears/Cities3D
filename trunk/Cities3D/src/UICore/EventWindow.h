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
#include "DataObject.h"
#include "UIExport.h"
#include "HtmlWindowEx.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxEventWindow
//
// The network event window.  Logs all game events.
// 
// Derived From:
//     <wxHtmlWindowEx>
//
// Project:
//     <UICore>
//
// Include:
//     EventWindow.h
//
class UI_EXPORT_SPEC wxEventWindow : public wxHtmlWindowEx
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxEventWindow
	//
	// The wxEventWindow constructor.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//     id - The window id.
	//     pos - The starting window position.
	//     size - The starting window size.
	//
	wxEventWindow(wxWindow* parent, wxWindowID id,
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//
	
	//-----------------------------------------------------------------------//
	// Destructor: ~wxEventWindow
	//
	// The wxEventWindow destructor.
	//
	virtual ~wxEventWindow();

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnSystemMessage
	//
	// Handles an incoming system message.
	//
	// Parameters:
	//     object - The system message to display.
	//
	void OnSystemMessage(const DataObject &object);
};

//---------------------------- PROTOTYPES       -----------------------------//

