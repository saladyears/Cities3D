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

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxBaseStaticText
// 
// A basic static text window.  A static text window displays
// read-only text on the screen.  The class wxBaseStaticText provides
// a simple base class for all static text windows in the game, 
// providing a common look and feel across UI objects.  It is in all
// ways like a wxStaticText, except that it sets its background to the
// background of its parent window, sets its foreground to the given
// color, and allows for a user-defined font.
//
// Derived From:
//     wxStaticText (see <http://www.wxwindows.org/manuals/2.4.2/wx362.htm>),
//
// Project:
//     <UICore>
//
// Include:
//     BaseStaticText.h
//
class UI_EXPORT_SPEC wxBaseStaticText : public wxStaticText
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxBaseStaticText
	//
	// The wxBaseStaticText constructor.  Creates the static text with the
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
	wxBaseStaticText(wxWindow* parent, wxWindowID id, const wxString& label, 
		const HashString &color = shDefaultText, 
		const HashString &font = shDefaultFont, 
		const wxSize& size = wxDefaultSize, long style = 0);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxBaseStaticText
	//
	// The wxBaseStaticText destructor.
	//
	~wxBaseStaticText();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: SetTextColor
	//
	// Sets the color of the text.
	//
	// Parameters:
	//     color - The new color.
	//
	void SetTextColor(const HashString& color)
	{
		mColor = color;
		OnSkin();
	}

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
	// Handles a skin change.
	//
	void OnSkin(wxInt32 = 0);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// String: mColor
	//
	// The text color of this item.
	//
	HashString mColor;
};

//---------------------------- PROTOTYPES       -----------------------------//
