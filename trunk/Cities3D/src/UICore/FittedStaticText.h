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
#include "BaseStaticText.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxFittedStaticText
//
// A static text window that calculates the height needed to display its
// contents when created and sets itself to that size.
//
// Derived From:
//     <wxBaseStaticText>
//
// Project:
//     <UICore>
//
// Include:
//     FittedStaticText.h
//
class UI_EXPORT_SPEC wxFittedStaticText : public wxBaseStaticText
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxFittedStaticText
	//
	// The wxFittedStaticText constructor.  Creates the static text with the
	// given text color, font, style, size, etc.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//     id - The message handling ID.  Should be a unique (to the parent 
	//          window) ID, if the parent window wants to receive messages from
	//          the control.
	//     label - The text.
	//     color - The color in the <SkinManager> for this control.
	//     font - The font in the <SkinManager> for this control.
	//     size - The window size.
	//     style - The window style.
	//
	wxFittedStaticText(wxWindow* parent, wxWindowID id, const wxString& label, 
		const HashString &color = shDefaultText, 
		const HashString &font = shDefaultFont, 
		const wxSize& size = wxDefaultSize, long style = 0);
};

//---------------------------- PROTOTYPES       -----------------------------//

