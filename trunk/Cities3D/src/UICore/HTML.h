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
#include "ColorInfoData.h"
#include "UIExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//
namespace HTML
{
	//-----------------------------------------------------------------------//
	// Function: ConvertColorToHex
	//
	// Converts the given color to a hex string.
	//
	// Parameters:
	//     color - The color to convert.
	//
	// Returns:
	//     A string that is the color in hex.
	//
	UI_EXPORT_SPEC wxString ConvertColorToHex(const wxColour color);

	//-----------------------------------------------------------------------//
	// Function: Font
	//
	// Wraps the given text in HTML tags that format it to the given font and
	// size.
	//
	// Parameters:
	//     text - [out] The text to be wrapped.
	//     size - The desired size.
	//     face - The desired font face.
	//     color - The desired font color.
	//
	// Returns:
	//     A reference to the given string.
	//
	UI_EXPORT_SPEC wxString &Font(wxString &text, 
		const HashString &size = shSystemMessageText, 
		const HashString &color = shSystemMessageText, 
		const HashString &font = shSystemMessageFont);

	//-----------------------------------------------------------------------//
	// Function: Color
	//
	// Wraps the given text in HTML tags that format it to the given color.
	//
	// Parameters:
	//     text - [out] The text to be wrapped.
	//     type - The color in question.
	//
	// Returns:
	//     A reference to the given string.
	//
	UI_EXPORT_SPEC wxString &Color(wxString &text, const ColorType type);

	//-----------------------------------------------------------------------//
	// Function: Bold
	//
	// Wraps the given text in HTML tags that format it to bold.
	//
	// Parameters:
	//     text - [out] The text to be wrapped.
	//
	// Returns:
	//     A reference to the given string.
	//
	UI_EXPORT_SPEC wxString &Bold(wxString &text);

	//-----------------------------------------------------------------------//
	// Function: Italics
	//
	// Wraps the given text in HTML tags that format it to italics.
	//
	// Parameters:
	//     text - [out] The text to be wrapped.
	//
	// Returns:
	//     A reference to the given string.
	//
	UI_EXPORT_SPEC wxString &Italics(wxString &text);

	//-----------------------------------------------------------------------//
	// Function: Underline
	//
	// Wraps the given text in HTML tags that format it to underline.
	//
	// Parameters:
	//     text - [out] The text to be wrapped.
	//
	// Returns:
	//     A reference to the given string.
	//
	UI_EXPORT_SPEC wxString &Underline(wxString &text);

	//-----------------------------------------------------------------------//
	// Function: Break
	//
	// Wraps the given text in HTML tags that ends with a line break.
	//
	// Parameters:
	//     text - [out] The text to be wrapped.
	//
	// Returns:
	//     A reference to the given string.
	//
	UI_EXPORT_SPEC wxString &Break(wxString &text);

	//-----------------------------------------------------------------------//
	// Function: Row
	//
	// Wraps the given text in HTML tags that make it a row in a table.
	//
	// Parameters:
	//     text - [out] The text to be wrapped.
	//
	// Returns:
	//     A reference to the given string.
	//
	UI_EXPORT_SPEC wxString &Row(wxString &text);

	//-----------------------------------------------------------------------//
	// Function: Col
	//
	// Wraps the given text in HTML tags that make it a column in a table.
	//
	// Parameters:
	//     text - [out] The text to be wrapped.
	//
	// Returns:
	//     A reference to the given string.
	//
	UI_EXPORT_SPEC wxString &Col(wxString &text);
}

//---------------------------- PROTOTYPES       -----------------------------//

