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
#include "ColorInfoData.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: KnightCount
// 
// A knight bitmap with a corresponding counter of available / active knights.
//
// Derived From:
//     <wxOwnerDrawWindow>
//
// Project:
//     <CitiesRules>
//
// Include:
//     KnightCount.h
//
class KnightCount : public wxOwnerDrawWindow
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	enum State
	{
		StateLosing			= 0,
		StateNeutral		,
		StateWinning		,
	};

	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: KnightCount
	//
	// The KnightCount constructor.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//     index - The index of the player.
	//     game - The <Game> data.
	//     pos - The starting position.
	//     size - The starting size.
	//
	KnightCount(wxWindow *parent, wxInt32 index, const GamePtr& game, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~KnightCount
	//
	// The KnightCount destructor.
	//
	virtual ~KnightCount();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	void SetState(State state);

	State GetState() const
	{
		return mState;
	}

	void SetPlayerColor(ColorType color);

	//-----------------------------------------------------------------------//
	// Function: Update
	//
	// Updates the counter text based on the given parameters.
	//
	// Parameters:
	//     game - The current <Game>.
	//
	void Update(const GamePtr &game);

	//-----------------------------------------------------------------------//
	// Function: Resize
	//
	// Resizes the KnightCount.
	//
	// Parameters:
	//     origin - The starting point of the KnightCount.
	//     bitmapSize - The size the bitmap should be.
	//     textHeight - The maximum text height.
	//
	void Resize(const wxSize &bitmapSize, wxInt32 textHeight);

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Paint
	//
	// Paints the window.
	//
	// Parameters:
	//     dc - The memory dc to draw into.
	//
	virtual void Paint(wxMemoryDC &dc);

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnSkin
	//
	// Handles skin changes.
	//
	void OnSkin(wxInt32=0);

	//-----------------------------------------------------------------------//
	// Function: OnImageUpdate
	//
	// Handles image set changes.
	//
	void OnImageUpdate(bool);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	typedef boost::shared_ptr<wxStaticBitmap> StaticBitmapPtr;
	typedef boost::shared_ptr<wxStaticText> StaticTextPtr;

	//-----------------------------------------------------------------------//
	// Int: mPlayerIndex
	//
	// The index of the player this count represents.
	//
	wxInt32 mPlayerIndex;

	wxInt32 mTextHeight;

	State mState;

	//-----------------------------------------------------------------------//
	// Int: mPlayerColor
	//
	// The color of the player this count represents.
	//
	ColorType mPlayerColor;

	//-----------------------------------------------------------------------//
	// Object: mBitmap
	//
	// The bitmap.
	//
	wxBitmap mBitmap;

	//-----------------------------------------------------------------------//
	// Object: mText
	//
	// The text.
	//
	wxString mText;
};

//---------------------------- PROTOTYPES       -----------------------------//

