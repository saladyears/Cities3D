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
#include "BaseImageButton.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxBaseButton
// 
// The standard bitmapped button in the game.  Uses
// button bitmaps retrieved from the <IImageEngine>.
//
// Derived From:
//     <wxBaseImageButton>
//
// Project:
//     <UICore>
//
// Include:
//     BaseButton.h
//
class UI_EXPORT_SPEC wxBaseButton : public wxBaseImageButton
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxBaseButton
	//
	// The wxBaseButton constructor.  
	//
	// Parameters:
	//     parent - The parent window (must not be NULL).
	//     id - The id for message handling.
	//     label - The button label.
	//
	wxBaseButton(wxWindow* parent, wxWindowID id, const wxString &label);
};

//---------------------------- PROTOTYPES       -----------------------------//

