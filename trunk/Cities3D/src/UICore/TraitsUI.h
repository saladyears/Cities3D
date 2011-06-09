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
#include <wx/hashmap.h>

//---------------------------- USER INCLUDES    -----------------------------//

//---------------------------- DEFINES          -----------------------------//
#include "UIExport.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: TraitsUI
// 
// Manages internal element sizes for a <wxBaseUI>-derived object. TraitsUI 
// provides a foundation for any <wxBaseUI> class to have a set of parameters
// that define a default size for the UI element as well as every single
// internal element within the overall element.  The TraitsUI class provides
// a set of hash maps that let any size value needed to draw the window
// be accessed via a text string, allowing great flexibility and extensibility.
//
// TraitsUI handles user-initiated resizes of every UI element by
// resetting itself back to its default state and then applying the resize
// percentage to every element in its size hash map, thus ensuring a linear
// size change for every element when the UI element is drawn.  By applying
// the same change percentage to every element, it allows the relative sizes 
// of the UI element to preserved, the goal being that the UI element
// looks good at any size.
//
// Project:
//     <UICore>
//
// Include:
//     TraitsUI.h
//
class UI_EXPORT_SPEC TraitsUI
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: size
	//
	// Returns the size value for the given key.  First
	// checks for the key in the constant hash.  Then
	// checks in the size hash, and finally throws
	// an error message to the screen if the key does not exist.
	//
	// Paramaters:
	//     key - The key for the size hash map.
	//
	// Returns:
	//     The size value associated with the key.
	//
	wxInt32 size(const HashString &key);

	//-----------------------------------------------------------------------//
	// Function: color
	//
	// Returns the color value for the given key.
	// Throws an error message to the screen if the key
	// does not exist.
	//
	// Paramaters:
	//     key - The key for the color hash map.
	//
	// Returns:
	//     The color value associated with the key.
	//
	const wxColour color(const HashString &key);

	//-----------------------------------------------------------------------//
	// Function: calculateFontSize
	//
	// Determines the best sized font for a given height.
	// Given a font value, a height, and the number of lines of text,
	// determines the size of the font that will fit that number of
	// lines of text into the given height, and then sets the font point size
	// to it.
	//
	// Parameters:
	//     dc - The device context the text is being drawn in.
	//     font - The key of the font in the font hash.
	//     height - The total height the font has to fit in.
	//     num - The number of lines within the total height.
	//
	const wxFont calculateFontSize(wxDC &dc, const HashString &font, 
		const HashString &height, const wxInt32 num = 1);

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Resize
	//
	// Resizes all values in the size hash.  Resets the
	// TraitsUI object to its default values and then
	// multiplies all values in the size hash by the given
	// percentage.
	//
	// Parameters:
	//     perecent - The percent to multiple the size hash values by.
	//
	virtual void Resize(const float percent);

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: TraitsUI
	//
	// The TraitsUI constructor.  Sets default size fields of Width and Height,
	// since all UI elements have at least a width and a height.  Sets default
	// colors for background, outline, and font color, so that all UI elements
	// have a common look and feel.  Sets the default font for all UI elements,
	// for the same reason.
	//
	TraitsUI();

	WX_DECLARE_HASH_STRING_HASH_MAP(wxInt32, IntHash);
	WX_DECLARE_HASH_STRING_HASH_MAP(HashString, StringHash);

	//-----------------------------------------------------------------------//
	// Destructor: TraitsUI
	//
	virtual ~TraitsUI() { }

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Hash: mSizes
	//
	// The size hash.  Contains UI element sizes that will
	// be resized if the user initiates a total UI resize event.
	//
	IntHash mSizes;

	//-----------------------------------------------------------------------//
	// Hash: mConstants
	//
	// Unchangeable integer data hash.  Any constants that
	// need to be saved through UI resizes are stored here.
	//
	IntHash mConstants;

	//-----------------------------------------------------------------------//
	// Hash: mColours
	//
	// The color hash.  Any colors needed for the UI element
	// are stored here.
	StringHash mColours;

	//-----------------------------------------------------------------------//
	// Hash: mFonts
	//
	// The font hash.  Any fonts needed for the UI element
	// are stored here.
	StringHash mFonts;

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Reset
	//
	// Resets the TraitsUI size hash back to default values.
	//
	virtual void Reset()=0;
};
//---------------------------- PROTOTYPES       -----------------------------//
typedef boost::shared_ptr<TraitsUI> TraitsUIPtr;
