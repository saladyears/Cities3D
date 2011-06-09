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

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxImageCenteredBitmap
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
//     ImageCenteredBitmap.h
//
class UI_EXPORT_SPEC wxImageCenteredBitmap : public wxCenteredBitmap
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxImageCenteredBitmap
	//
	// The wxImageCenteredBitmap constructor.
	//
	// Parameters:
	//
	//     parent - The parent window.
	//     size - The starting size of the window.
	//     bitmapSize - The starting size of the bitmap.
	//     key - The key in the image engine to use for the bitmaps.
	//     bitmap - The starting bitmap image in the image engine.
	//
	wxImageCenteredBitmap(wxWindow* parent, const wxSize& size, 
		const wxSize& bitmapSize, const HashString& key, 
		const HashString& bitmap, bool display = true);

	const HashString& GetBitmapImage() const
	{
		return mBitmap;
	}

	void SetBitmapImage(const HashString& image);

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	virtual wxBitmap GetBitmap(const wxSize& size);

	HashString mKey;
	HashString mBitmap;
};

//---------------------------- PROTOTYPES       -----------------------------//

