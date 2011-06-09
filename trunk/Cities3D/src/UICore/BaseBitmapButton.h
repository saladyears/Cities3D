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

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxBaseBitmapButton
// 
// A completely custom-drawn bitmapped button.  The class wxBaseBitmapButton
// provides a number of features: When the user mouses over, if the button is 
// enabled, the cursor is changed to a hand, indicating the user can click the 
// button.  When clicked, the button shifts itself one pixel right and down, to
// give visual feedback that is in the down state.  The click message to the 
// parent is not sent unless the mouse button is released, and the mouse is 
// still over the button.  This emulates the Windows OS.  The button is drawn 
// using memory bitmaps, to completely remove flicker.  Finally, the font used 
// to draw the label on the button is resized if the button is resized so that 
// its height is always around 70% or less of the available button height, 
// giving a consistent look, even on differently sized buttons.
//
// Derived From:
//     <wxOwnerDrawWindow>
//
// Project:
//     <UICore>
//
// Include:
//     ClearButton.h
//
class UI_EXPORT_SPEC wxBaseBitmapButton : public wxOwnerDrawWindow
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Enable
	//
	// Enables or disables the button.
	//
	// Parameters:
	//     enable - The enable flag.  True if the button should be
	//              enabled, false if it should be disabled.
	//
	bool Enable(bool enable = true);

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxBaseBitmapButton
	//
	// The wxBaseBitmapButton constructor.  Creates the button with
	// the given label and bitmaps.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//     id - The message handling ID.  Should be a unique (to the parent 
	//          window) ID, if the parent window wants to receive messages from
	//          the control.
	//     pos - The starting position of the button.
	//     size - The starting size of the button.
	//
	wxBaseBitmapButton(wxWindow* parent, wxWindowID id, const wxPoint& pos,
		const wxSize& size);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxBaseBitmapButton
	//
	// The wxBaseBitmapButton destructor.
	//
	virtual ~wxBaseBitmapButton();

	//-----------------------------------------------------------------------//
	// Group: Enumerations
	//

	//-----------------------------------------------------------------------//
	// Enum: Mode
	//
	// Mouse handling flags.  Used when the user is holding
	// down the left mouse button while moving the mouse around.
	// Flags are: *ModeNone*, *ModeDown*, *ModeDownOutside*.
	//
	enum Mode
	{
		ModeNone,
		ModeDown,
		ModeDownOutside,
	};

	//-----------------------------------------------------------------------//
	// Function: SetBitmaps
	//
	// Sets the bitmaps for the button.
	//
	// Parameters:
	//     enabled - The enabled bitmap.
	//     disabled - The disabled bitmap.
	//
	void SetBitmaps(const wxBitmap& enabled, const wxBitmap& disabled);

	//-----------------------------------------------------------------------//
	// Function: GetMode
	//
	// Returns the current mode of the bitmap.
	//
	// Returns:
	//     The current mode of the bitmap.
	//
	Mode GetMode() const
	{
		return mMode;
	}

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
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Macro: DECLARE_EVENT_TABLE
	//
	// The wxWidgets event table declaration.
	//
	DECLARE_EVENT_TABLE()

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: SetBitmap
	//
	// Scales and sets the correct draw bitmap based on the enabled state.
	//
	void SetBitmap();

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
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnEnterWindow
	//
	// Handles the mouse entering the button's rectangle.
	// Used to keep track of when the mouse has come back in
	// to the button space after leaving while the left mouse
	// button is down.
	//
	// Parameters:
	//     event - The mouse event.
	//
	void OnEnterWindow(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnLeftDown
	//
	// Handles the left mouse down event.  Puts the button
	// in ModeDown state.
	// 
	// Parameters:
	//     event - The mouse event.
	//
	void OnLeftDown(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnLeftUp
	//
	// Handles the left mouse up event.  Sends a click message
	// to the parent if the button is in ModeDown state.
	//
	// Parameters:
	//     event - The mouse event.
	//
	void OnLeftUp(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnLeaveWindow
	//
	// Handles the mouse leaving the button's rectangle.
	// Used to keep track of when the mouse has leaves the
	// button space while the left mouse button is down.
	//
	// Parameters:
	//     event - The mouse event.
	//
	void OnLeaveWindow(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnMouseMove
	//
	// Handles the mouse moving over the button.
	//
	// Parameters:
	//     event - The mouse event.
	//
	void OnMouseMove(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Object: mDrawBitmap
	//
	// The currently drawn bitmap.
	//
	wxBitmap mDrawBitmap;

	//-----------------------------------------------------------------------//
	// Object: mEnabledBitmap
	//
	// The enabled bitmap.  Drawn when the bitmap is in
	// the enabled state.
	//
	wxBitmap mEnabledBitmap;

	//-----------------------------------------------------------------------//
	// Object: mDisabledBitmap
	//
	// The disabled bitmap.  Drawn when the bitmap is in
	// the disabled state.
	//
	wxBitmap mDisabledBitmap;

	//-----------------------------------------------------------------------//
	// Object: mMode
	//
	// The button mode.
	//
	Mode mMode;
};

//---------------------------- PROTOTYPES       -----------------------------//

