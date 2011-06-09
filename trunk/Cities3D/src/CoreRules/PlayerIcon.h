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
#include "PlayerData.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: PlayerIcon
// 
// A piece of UI in the <PlayerUI>, a bitmap representing some state.
//
// Project:
//     <Cities3D>
//
// Include:
//     PlayerIcon.h
//
class PlayerIcon
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: PlayerIcon
	//
	// The PlayerIcon constructor.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//     update - The update function that sets the counter.
	//     key - The key in the image engine for this bitmap.
	//     text - The popup text for the element.
	//
	PlayerIcon(wxWindow *parent, const PlayerUpdateFunc &update, 
		const HashString& key, const wxString& text);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~PlayerIcon
	//
	// The PlayerIcon destructor.
	//
	virtual ~PlayerIcon();

	//-----------------------------------------------------------------------//
	// Function: Update
	//
	// Updates the counter text based on the given parameters.
	//
	// Parameters:
	//     game - The current <Game>.
	//     index - The player index.
	//
	void Update(const GamePtr &game, const wxInt32 index);

	//-----------------------------------------------------------------------//
	// Function: Resize
	//
	// Resizes the PlayerIcon.
	//
	// Parameters:
	//     origin - The starting point of the PlayerIcon.
	//     bitmapSize - The size the bitmap should be.
	//
	void Resize(const wxPoint &origin, const wxSize &bitmapSize);

	//-----------------------------------------------------------------------//
	// Function: ResetBitmap
	//
	// Resets the bitmap.
	//
	void ResetBitmap();

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnSkin
	//
	// Handles skin changes.
	//
	void OnSkin(wxInt32 = 0);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Object: mUpdateFunc
	//
	// The update function that sets the value of the counter text.
	//
	PlayerUpdateFunc mUpdateFunc;

	typedef boost::shared_ptr<wxStaticBitmap> StaticBitmapPtr;

	//-----------------------------------------------------------------------//
	// Object: mBitmap
	//
	// The bitmap.
	//
	StaticBitmapPtr mBitmap;

	//-----------------------------------------------------------------------//
	// Object: mKey
	//
	// The image key.
	//
	HashString mKey;
};

//---------------------------- PROTOTYPES       -----------------------------//

