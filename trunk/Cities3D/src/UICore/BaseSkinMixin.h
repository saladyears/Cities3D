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
#include "SkinManager.h"
#include "Controller.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: BaseSkinMixin
//
// A template mixin class that provides default OnSkin handling for a skin 
// changing, setting the font, text color, and background color.
//
// Project:
//     <UICore>
//
// Include:
//     BaseSkinMixin.h
//
template <typename T>
class BaseSkinMixin
{
	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: BaseSkinMixin
	//
	// The BaseSkinMixin constructor.  Stores the given color, backgroundColor
	// and font strings for lookup in the <SkinManager>. If backgroundColor
	// is omitted, the default shBackground will be used.
	//
	BaseSkinMixin(const HashString &color, const HashString &font,
		const HashString &backgroundColor = shBackground)
	: mColor(color)
	, mFont(font)
	, mBackgroundColor(backgroundColor)
	{
		Controller::get().AddReceiver(shEventSkin, &BaseSkinMixin::OnSkin, 
			this);
	}

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~BaseSkinMixin
	//
	// The BaseSkinMixin destructor.
	//
	virtual ~BaseSkinMixin()
	{
		Controller::get().RemoveReceiver(shEventSkin, &BaseSkinMixin::OnSkin, 
			this);
	}

	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnSkin
	//
	// Handles a new skin being selected.
	//
	void OnSkin(wxInt32 = 0)
	{
		T *thisWindow = const_cast<T *>(static_cast<const T *>(this));

		thisWindow->SetBackgroundColour(SKIN.Element(mBackgroundColor));
		thisWindow->SetForegroundColour(SKIN.Element(mColor));
		thisWindow->SetFont(SKIN.Font(mFont));

		thisWindow->Refresh();
	}

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// String: mBackgroundColor
	//
	// The background color.
	//
	HashString mBackgroundColor;

	//-----------------------------------------------------------------------//
	// String: mColor
	//
	// The color of the text.
	//
	HashString mColor;

	//-----------------------------------------------------------------------//
	// String: mFont
	//
	// The font of the text.
	//
	HashString mFont;
};

//---------------------------- PROTOTYPES       -----------------------------//

