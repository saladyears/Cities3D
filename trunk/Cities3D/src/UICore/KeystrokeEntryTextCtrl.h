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
#include "KeystrokeEntryTextCtrl.h"
#include "BaseTextCtrl.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

class UI_EXPORT_SPEC IKeystrokeHandler
{
public:
    virtual ~IKeystrokeHandler() {}
    virtual void HandleKeyEvent(const wxKeyEvent& event, bool keydown)=0;
};

//---------------------------------------------------------------------------//
// Class: wxKeystrokeEntryTextCtrl
// 
// A text control that reports a full keystroke to its parent, used for 
// mapping user key strokes to game events.
//
// Derived From:
//     <wxBaseTextCtrl>
//
// Project:
//     <UICore>
//
// Include:
//     KeystrokeEntryTextCtrl.h
//
class UI_EXPORT_SPEC wxKeystrokeEntryTextCtrl : public wxBaseTextCtrl
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxKeystrokeEntryTextCtrl
	//
	// The wxKeystrokeEntryTextCtrl constructor.  Creates the text control 
    // with the given position, style, size, etc.
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
	wxKeystrokeEntryTextCtrl(wxWindow* parent, wxWindowID id, 
        IKeystrokeHandler* keystrokeHandler,
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
	// Function: OnKeyDown
	//
	// Handles the user pressing a key.
	//
	// Parameters:
	//     event - The key event.
	//
	void OnKeyDown(wxKeyEvent& event);

    //-----------------------------------------------------------------------//
	// Function: OnKeyUp
	//
	// Handles the user pressing a key.
	//
	// Parameters:
	//     event - The key event.
	//
	void OnKeyUp(wxKeyEvent& event);

    //-----------------------------------------------------------------------//
	// Group: Variables
	//

    //-----------------------------------------------------------------------//
	// Pointer: mKeystrokeHandler
	//
    // The keystroke handle to pass keystroke events to.
    //
    IKeystrokeHandler* mKeystrokeHandler;
};

//---------------------------- PROTOTYPES       -----------------------------//
