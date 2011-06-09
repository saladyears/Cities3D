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
#include "PlayerElement.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: PlayerUIElement
// 
// A <PlayerElement> that uses bitmaps stored in the <ImageEngine>.
//
// Derived From:
//     <PlayerElement>
//
// Project:
//     <Cities3D>
//
// Include:
//     PlayerUIElement.h
//
class PlayerUIElement : public PlayerElement
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: PlayerUIElement
	//
	// The PlayerUIElement constructor.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//     update - The update function that sets the counter.
	//     color - The update function that sets the color of the counter.
	//     right - A bool that is true if the counter text is displayed to the
	//             right of the bitmap and false if not.
	//     threshold - The counter value at which the PlayerElement should be
	//                 hidden.
	//     imageKey - The image key in the <ImageEngine>.
	//     imageName - The image name in the <ImageEngine>.
	//     text - The popup text for the element.
	//     card - Whether the element is a card element.
	//     index - The index of the player.
	//
	PlayerUIElement(wxWindow *parent, const PlayerUpdateFunc &update, 
		const PlayerColorFunc &color, bool right, wxInt32 threshold,
		const HashString &imageKey, const HashString &imageName,
		const wxString& text, bool card, wxInt32 index);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~PlayerUIElement
	//
	// The PlayerUIElement destructor.
	//
	virtual ~PlayerUIElement();

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: GetBitmap
	//
	// Returns the bitmap from the <ImageEngine> at the given size.
	//
	// Parameters:
	//     size - The size of the bitmap.
	//
	virtual wxBitmap GetBitmap(const wxSize &size);

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnImageUpdate
	//
	// Handles a change in the game images used.
	//
	void OnImageUpdate(bool);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// String: mImageKey
	//
	// The key in the <ImageEngine> of the bitmap.
	//
	HashString mImageKey;

	//-----------------------------------------------------------------------//
	// String: mImageName
	//
	// The name in the <ImageEngine> of the bitmap.
	//
	HashString mImageName;
};
//---------------------------- PROTOTYPES       -----------------------------//

