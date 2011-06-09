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

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxHtmlWindowEx;
class wxChatInputTextCtrl;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxChatWindow
//
// The network chat window.  Contains two sub windows, one that displays the
// chat log, the other which allows chat text entry.
// 
// Derived From:
//     <wxWindow>
//
// Project:
//     <UICore>
//
// Include:
//     ChatWindow.h
//
class UI_EXPORT_SPEC wxChatWindow : public wxWindow
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxChatWindow
	//
	// The wxChatWindow constructor.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//     id - The window id.
	//     entry - Whether the chat window should have a text entry control.
	//     pos - The starting window position.
	//     size - The starting window size.
	//
	wxChatWindow(wxWindow* parent, wxWindowID id, const bool entry = true,
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//
	
	//-----------------------------------------------------------------------//
	// Destructor: ~wxChatWindow
	//
	// The wxChatWindow destructor.
	//
	virtual ~wxChatWindow();

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
	// Function: OnChatMessage
	//
	// Handles an incoming chat message.
	//
	// Parameters:
	//     object - The chat message to display.
	//
	void OnChatMessage(const DataObject &object);

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnSize
	//
	// Resizes the window.
	// 
	// Parameters:
	//     event - The size event.
	//
	void OnSize(wxSizeEvent& event);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Pointer: mpHtmlWindow
	//
	// The HTML chat window.
	//
	wxHtmlWindowEx *mpHtmlWindow;

	//-----------------------------------------------------------------------//
	// Pointer: mpInputText
	//
	// The chat entry window.
	//
	wxChatInputTextCtrl *mpInputText;
};

//---------------------------- PROTOTYPES       -----------------------------//

