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
#include <wx/treectrl.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "UIExport.h"
#include "BaseSkinMixin.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxBaseTreeCtrl
// 
// A basic tree control.  A tree control displays items in a tree
// hierarchy with node selection and expansion.  Icons may also be
// added to each node in the tree. The class wxBaseTreeCtrl provides
// a simple base class for all tree controls in the game, 
// providing a common look and feel across UI objects.  It is in all
// ways like a wxTreeCtrl, except that it sets its background to the
// background of its parent window, and sets its foreground color to
// white.
//
// Derived From:
//     wxTreeCtrl (see <http://www.wxwindows.org/manuals/2.4.2/wx399.htm>),
//     <BaseSkinMixin>
//
// Project:
//     <UICore>
//
// Include:
//     BaseTreeCtrl.h
//
class UI_EXPORT_SPEC wxBaseTreeCtrl 
	: public wxTreeCtrl
	, public BaseSkinMixin<wxBaseTreeCtrl>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxBaseTreeCtrl
	//
	// The wxBaseTreeCtrl constructor.  Creates the tree control with the
	// given position, size, etc.
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
	wxBaseTreeCtrl(wxWindow* parent, wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		const wxInt32 style = wxTR_HIDE_ROOT | wxTR_NO_BUTTONS | 
		wxTR_NO_LINES | wxTR_FULL_ROW_HIGHLIGHT | wxTR_SINGLE,
		const HashString &color = shDefaultText, 
		const HashString &font = shDefaultFont
		);
};

//---------------------------- PROTOTYPES       -----------------------------//
