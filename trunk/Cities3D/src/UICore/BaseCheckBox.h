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
// Class: wxBaseCheckBox
// 
// A basic check box window.  A check box window displays
// a two-state check box with text.  The class wxBaseCheckBox provides
// a simple base class for all check box windows in the game, 
// providing a common look and feel across UI objects.  It is in all
// ways like a wxCheckBox, except that it sets its background to the
// background of its parent window, sets its foreground to the given
// color, and allows for a user-defined font.
//
// Derived From:
//     wxCheckBox (see <http://www.wxwindows.org/manuals/2.4.2/wx52.htm>),
//     <BaseSkinMixin>
//
// Project:
//     <UICore>
//
// Include:
//     BaseCheckBox.h
//
class UI_EXPORT_SPEC wxBaseCheckBox 
	: public wxCheckBox
	, public BaseSkinMixin<wxBaseCheckBox>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxBaseCheckBox
	//
	// The wxBaseCheckBox constructor.  Creates the static text with the
	// given text font, color, etc.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//     id - The message handling ID.  Should be a unique (to the parent 
	//          window) ID, if the parent window wants to receive messages from 
	//          the control.
	//     label - The text.
	//     color - The text color.
	//     font - The text font.
	//
	wxBaseCheckBox(wxWindow* parent, wxWindowID id, const wxString& label, 
		const HashString &color = shDefaultText, 
		const HashString &font = shDefaultFont);
};

//---------------------------- PROTOTYPES       -----------------------------//
