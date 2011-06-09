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
#include "ColorInfoData.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: PlayerElement
// 
// A piece of UI in the <PlayerUI>, usually a bitmap with a corresponding
// counter.
//
// Project:
//     <Cities3D>
//
// Include:
//     PlayerElement.h
//
class PlayerElement
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: SetPlayerColor
	//
	// Allows derived classes to handle player color changes.
	//
	virtual void SetPlayerColor(const ColorType) {}

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Show
	//
	// Shows or hides the window from an external perspective.
	//
	// Parameters:
	//     show - A bool that is true if the window should be shown and false
	//            if not.
	//
	void Show(const bool show);

	//-----------------------------------------------------------------------//
	// Function: SetFont
	//
	// Sets the font.
	//
	// Parameters:
	//     font - The font to use.
	//
	void SetFont(const wxFont &font);

	//-----------------------------------------------------------------------//
	// Function: SetRight
	//
	// Sets the text location flag.
	//
	// Parameters:
	//     right - A bool that is true if the text should be drawn to the 
	//             right of the bitmap and false if not.
	//
	void SetRight(const bool right);

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
	// Resizes the PlayerElement.
	//
	// Parameters:
	//     origin - The starting point of the PlayerElement.
	//     bitmapSize - The size the bitmap should be.
	//     textSize - The size the text area should be.
	//
	void Resize(const wxPoint &origin, const wxSize &bitmapSize, 
		const wxSize textSize);

	//-----------------------------------------------------------------------//
	// Function: ResetBitmap
	//
	// Resets the bitmap.
	//
	void ResetBitmap();

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: PlayerElement
	//
	// The PlayerElement constructor.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//     update - The update function that sets the counter.
	//     color - The update function that sets the color of the counter.
	//     right - A bool that is true if the counter text is displayed to the
	//             right of the bitmap and false if not.
	//     threshold - The counter value at which the PlayerElement should be
	//                 hidden.
	//     text - The popup text for the element.
	//     card - Whether the element is a card element.
	//     index - The index of the player.
	//
	PlayerElement(wxWindow *parent, const PlayerUpdateFunc &update, 
		const PlayerColorFunc &color, bool right, const wxInt32 threshold,
		const wxString& text, bool card = false, wxInt32 index = -1);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~PlayerElement
	//
	// The PlayerElement destructor.
	//
	virtual ~PlayerElement();

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: GetBitmap
	//
	// Gets the bitmap to be drawn.
	//
	// Parameters:
	//     size - The size of the bitmap.
	//
	virtual wxBitmap GetBitmap(const wxSize &size)=0;

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
	// Function: ShowPrivate
	//
	// Shows or hides the window based on the internal and external show flags.
	//
	void ShowPrivate();

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Object: mUpdateFunc
	//
	// The update function that sets the value of the counter text.
	//
	PlayerUpdateFunc mUpdateFunc;

	//-----------------------------------------------------------------------//
	// Object: mColorFunc
	//
	// The color function that sets the color of the counter text.
	//
	PlayerColorFunc mColorFunc;

	//-----------------------------------------------------------------------//
	// Bool: mShowExternal
	//
	// A flag that is true if the PlayerUI wants the element to be shown and
	// false if not.
	//
	bool mShowExternal;

	//-----------------------------------------------------------------------//
	// Bool: mShowInternal
	//
	// A flag that is true if the update function wants the element to be
	// shown and false if not.
	//
	bool mShowInternal;

	//-----------------------------------------------------------------------//
	// Bool: mRight
	//
	// A flag that is true if the text should be shown to the right of the
	// bitmap and false if it should be shown underneath.
	//
	bool mRight;

	//-----------------------------------------------------------------------//
	// Int: mThreshold
	//
	// The cutoff value at which point the PlayerElement should be hidden.
	//
	wxInt32 mThreshold;

	typedef boost::shared_ptr<wxStaticBitmap> StaticBitmapPtr;
	typedef boost::shared_ptr<wxStaticText> StaticTextPtr;

	//-----------------------------------------------------------------------//
	// Object: mBitmap
	//
	// The bitmap.
	//
	StaticBitmapPtr mBitmap;

	//-----------------------------------------------------------------------//
	// Object: mText
	//
	// The text.
	//
	StaticTextPtr mText;
};

//---------------------------- PROTOTYPES       -----------------------------//

