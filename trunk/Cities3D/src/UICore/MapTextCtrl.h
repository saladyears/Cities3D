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
#include "HtmlWindowEx.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class Game;
typedef boost::shared_ptr<Game> GamePtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxMapTextCtrl
// 
// Displays information about the currently selected map in a text
// control.  This includes all of the relevant map information, such as
// map name, points, min/max players, map creator, etc.  The wxMapTextCtrl
// also includes information about any variants that are in effect, and
// shows whether the map is Standard, Seafarers, Cities & Knights, or any
// combination thereof.
//
// Derived From:
//     <wxHtmlWindowEx>
//
// Project:
//     <UICore>
//
// Include:
//     MapTextCtrl.h
//
class UI_EXPORT_SPEC wxMapTextCtrl : public wxHtmlWindowEx
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxMapTextCtrl
	// 
	// The wxMapTextCtrl constructor.  Adds OnUpdateText as a 
	// receiver of the eventPreGame <Event>.  Immediately calls 
	// the MapDatabase to retrieve the current map selection text.
	//
	// Parameters:
	//     parent - The parent window.
	//     id - The message handling ID.  Should be a unique (to the parent
	//          window) ID, if the parent window wants to receive messages
	//          from the control.
	//     pos - The text control position in window coordinates.
	//     size - The text control size.
	//
	wxMapTextCtrl(wxWindow* parent, wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxMapTextCtrl
	// 
	// The wxMapTextCtrl destructor.  Removes OnUpdateText as 
	// a receiver of the eventPreGame <Event>.
	//
	~wxMapTextCtrl();

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:

	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnUpdateText
	//
	// Updates the text in the control.  Calls the <MapDatabase>
	// to retrieve the most up-to-date text.  Triggered by the
	// eventPreGame <Event>.
	//
	// Parameters:
	//     game - The current <Game>.
	//
	void OnUpdateText(const GamePtr &game);
};

//---------------------------- PROTOTYPES       -----------------------------//
