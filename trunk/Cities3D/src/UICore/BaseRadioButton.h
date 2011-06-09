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
// Class: wxBaseRadioButton
// 
// A basic radio button window.  A radio button window displays
// a two-state check box with text, and allows for groups of buttons
// to be created, where only one button in the group is selected at
// any given time.  The class wxBaseRadioButton provides
// a simple base class for all radio button windows in the game, 
// providing a common look and feel across UI objects.  It is in all
// ways like a wxRatioButton, except that it sets its background to the
// background of its parent window, sets its foreground to white.
//
// Derived From:
//     wxRadioButton (see <http://www.wxwindows.org/manuals/2.4.2/wx319.htm>),
//     <BaseSkinMixin>
//
// Project:
//     <UICore>
//
// Include:
//     BaseRadioBox.h
//
class UI_EXPORT_SPEC wxBaseRadioButton 
	: public wxRadioButton
	, public BaseSkinMixin<wxBaseRadioButton>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxBaseRadioButton
	//
	// The wxBaseRadioButton constructor.  Creates the radio button
	// with the given parameters
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//     id - The message handling ID.  Should be a unique (to the parent 
	//          window) ID, if the parent window wants to receive messages from
	//          the control.
	//     label - The text.
	//     pos - The radio button position in window coordinates.
	//     size - The radio button size.
	//     style - The radio button style.
	//
	wxBaseRadioButton(wxWindow* parent, wxWindowID id, const wxString& label,
		const HashString &color = shDefaultText,
		const HashString &font = shDefaultFont,
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, long style = 0);
};

//---------------------------- PROTOTYPES       -----------------------------//
