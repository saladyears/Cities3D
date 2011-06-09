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
#include "BaseBitmapButton.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxBaseImageButton
// 
// Enhances <wxBaseBitmapButton> by retrieving bitmaps from the <ImageEngine>
// and adding text to the button.
//
// Derived From:
//     <wxBaseBitmapButton>
//
// Project:
//     <UICore>
//
// Include:
//     BaseImageButton.h
//
class UI_EXPORT_SPEC wxBaseImageButton : public wxBaseBitmapButton
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxBaseImageButton
	//
	// The wxBaseImageButton constructor.  Creates the button with
	// the given label and bitmaps.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//     id - The message handling ID.
	//     label - The button text.
	//     size - The starting size of the button.
	//     key - The location in the <ImageEngine> for the bitmaps.
	//     enabled - The bitmap for the button when it is enabled.
	//     disabled - The bitmap for the button when it is disabled.
	//
	wxBaseImageButton(wxWindow* parent, wxWindowID id, const wxString& label, 
		const wxSize size, const HashString &key, const HashString &enabled, 
		const HashString &disabled = shEmpty);

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: SetLabel
	//
	// Sets the button text.
	//
	// Parameters:
	//     label - The new text of the button.
	//
	void SetLabel(const wxString &label);

	//-----------------------------------------------------------------------//
	// Function: SetBitmaps
	//
	// Sets the bitmaps.
	//
	// Parameters:
	//     enabled - The new enabled bitmap.
	//     disabled - The new disabled bitmap.
	//
	void SetBitmaps(const HashString &enabled, const HashString &disabled = shEmpty);

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Size
	//
	// Resizes the window.
	//
	// Parameters:
	//     event - The resize event.
	//
	virtual void Size(wxSizeEvent &event);

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: GetBitmaps
	//
	// Retrives the bitmaps from the <ImageEngine>.
	//
	void GetBitmaps();

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Paint
	//
	// Paints the button using the disabled/enabled bitmap
	// image and the label text.
	//
	// Parameters:
	//     dc - The memory device context to draw to.
	//
	virtual void Paint(wxMemoryDC &dc);

	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnSkin
	//
	// Handles a new skin being selected.
	//
	virtual void OnSkin(wxInt32 = 0);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Object: mLabel
	//
	// The button text.
	//
	wxString mLabel;

	//-----------------------------------------------------------------------//
	// Object: mKey
	//
	// The location in the <ImageEngine> for the bitmaps.
	//
	HashString mKey;

	//-----------------------------------------------------------------------//
	// Object: mEnabled
	//
	// The name of the enabled bitmap.
	//
	HashString mEnabled;

	//-----------------------------------------------------------------------//
	// Object: mDisabled
	//
	// The name of the disabled bitmap.
	//
	HashString mDisabled;

	typedef std::pair<wxString, wxPoint> LabelPair;
	typedef std::vector<LabelPair> LabelArray;

	//-----------------------------------------------------------------------//
	// Object: mLabelArray
	//
	// The actual text to be drawn, with its starting location on the button.
	//
	LabelArray mLabelArray;

	//-----------------------------------------------------------------------//
	// Object: mFont
	//
	// The desired draw font.
	//
	wxFont mFont;

	//-----------------------------------------------------------------------//
	// Object: mDrawFont
	//
	// The font the button text is actually drawn with.
	//
	wxFont mDrawFont;
};

//---------------------------- PROTOTYPES       -----------------------------//

