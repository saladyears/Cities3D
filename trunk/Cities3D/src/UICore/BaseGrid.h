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
#include <wx/grid.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "UIExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxBaseGrid
// 
// A basic grid.  A grid provides a tabular display of data.
//
// Derived From:
//     wxGrid
//     <BaseSkinMixin>
//
// Project:
//     <UICore>
//
// Include:
//     BaseGrid.h
//
class UI_EXPORT_SPEC wxBaseGrid : public wxGrid
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxBaseGrid
	//
	// The wxBaseGrid constructor.  Creates the grid with the
	// given parameters.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//     id - The message handling ID.  Should be a unique (to the parent 
	//          window) ID, if the parent window wants to receive messages from
	//          the control.
	//     pos - The tree control position in window coordinates.
	//     size - The tree control size.
	//
	wxBaseGrid(wxWindow* parent, wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize,
		const HashString &color = shDefaultText, 
		const HashString &font = shDefaultFont,
        const HashString &labelFont = shBoldFont);

    //-----------------------------------------------------------------------//
	// Group: Destructor
	//

    //-----------------------------------------------------------------------//
	// Destructor: ~wxBaseGrid
	//
    // The wxBaseGrid destructor.
    //
    ~wxBaseGrid();

    //-----------------------------------------------------------------------//
	// Section: Private
	//
private:
    //-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnSkin
	//
	// Handles a new skin being selected.
	//
	void OnSkin(wxInt32 = 0);

    //-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// String: mColor
	//
	// The color of the grid text.
	//
	HashString mColor;

	//-----------------------------------------------------------------------//
	// String: mFont
	//
	// The font of the grid text.
	//
	HashString mFont;

    //-----------------------------------------------------------------------//
	// String: mLabelFont
	//
	// The font of the grid label text.
	//
	HashString mLabelFont;
};

//---------------------------- PROTOTYPES       -----------------------------//
