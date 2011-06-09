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
// Class: wxBaseComboBox
// 
// A basic combo box.  A combo box provides a drop down list of items
// that the user may select  from.  The class wxBaseComboBox provides
// a simple base class for all combo boxes in the game, 
// providing a common look and feel across UI objects.  It is in all
// ways like a wxComboBox, except that it sets its background to the
// background of its parent window, and sets its foreground color to
// white.
//
// Derived From:
//     wxComboBox (see <http://www.wxwidgets.org/manuals/2.4.2/wx67.htm>),
//     <BaseSkinMixin>
//
// Project:
//     <UICore>
//
// Include:
//     BaseComboBox.h
//
class UI_EXPORT_SPEC wxBaseComboBox 
	: public wxComboBox
	, public BaseSkinMixin<wxBaseComboBox>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxBaseComboBox
	//
	// The wxBaseComboBox constructor.  Creates the combo box with the
	// given parameters.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//     id - The message handling ID.  Should be a unique (to the parent 
	//          window) ID, if the parent window wants to receive messages from
	//          the control.
	//     pos - The tree control position in window coordinates.
	//     size - The tree control size.
	//     style - The tree control style.
	//
	wxBaseComboBox(wxWindow* parent, wxWindowID id, 
		const wxString& value = swEmpty, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, wxInt32 n = 0, 
		const wxString choices[] = NULL, const wxInt32 style = 0,
		const HashString &color = shDefaultText, 
		const HashString &font = shDefaultFont);
};

//---------------------------- PROTOTYPES       -----------------------------//
