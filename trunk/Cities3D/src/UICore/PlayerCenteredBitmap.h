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
#include "CenteredBitmap.h"
#include "ColorInfoData.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxPlayerCenteredBitmap
//
// Centers and maintains aspect ratio of the given bitmap.
//
// Derived From:
//     <wxCenteredBitmap>
//
// Project:
//     <CitiesRules>
//
// Include:
//     PlayerCenteredBitmap.h
//
class UI_EXPORT_SPEC wxPlayerCenteredBitmap : public wxCenteredBitmap
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxPlayerCenteredBitmap
	//
	// The wxPlayerCenteredBitmap constructor.
	//
	// Parameters:
	//
	//     parent - The parent window.
	//     size - The starting size of the window.
	//     bitmapSize - The starting size of the bitmap.
	//     color - The color of this player.
	//
	wxPlayerCenteredBitmap(wxWindow* parent, const wxSize& size, 
		const wxSize& bitmapSize, ColorType color, bool display = true);

	void SetPlayerColor(ColorType color);

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	virtual wxBitmap GetBitmap(const wxSize& size);

	ColorType mColor;
};

//---------------------------- PROTOTYPES       -----------------------------//

