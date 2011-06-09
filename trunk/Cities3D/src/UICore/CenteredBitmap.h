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
#include "OwnerDrawWindow.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxCenteredBitmap
//
// Centers and maintains aspect ratio of the given bitmap.
//
// Derived From:
//     <wxOwnerDrawWindow>
//
// Project:
//     <CitiesRules>
//
// Include:
//     CenteredBitmap.h
//
class UI_EXPORT_SPEC wxCenteredBitmap : public wxOwnerDrawWindow
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxCenteredBitmap
	//
	// The wxCenteredBitmap constructor.
	//
	// Parameters:
	//
	//     parent - The parent window.
	//     size - The starting size of the window.
	//     bitmapSize - The starting size of the bitmap.
	//
	wxCenteredBitmap(wxWindow* parent, const wxSize& size, 
		const wxSize& bitmapSize, bool display = true);

	void Display(bool display)
	{
		mDisplay = display;
		Refresh();
	}

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:

	void Update();

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	virtual void Size(wxSizeEvent &event);

	virtual void Paint(wxMemoryDC &dc);

	virtual wxBitmap GetBitmap(const wxSize& size)=0;

	bool mDisplay;

	wxSize mOriginalSize;
	wxSize mOriginalBitmapSize;

	wxBitmap mDrawBitmap;
};

//---------------------------- PROTOTYPES       -----------------------------//

