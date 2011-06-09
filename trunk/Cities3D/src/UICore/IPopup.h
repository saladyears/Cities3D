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
// Class: IPopup
// 
// Base class for all windows that support popups.
//
// Project:
//     <UICore>
//
// Include:
//     IPopup.h
//
class UI_EXPORT_SPEC IPopup
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	enum PopupType
	{
		PopupAlwaysVisible,
		PopupRequestOnly,
	};

	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Type
	//
	// Returns the type of popup this is.
	//
	// Returns:
	//     The type of popup this is.
	//
	virtual PopupType Type() const=0;

	//-----------------------------------------------------------------------//
	// Function: TestPopup
	//
	// Tests whether the given mouse point (in screen coords) is on a window
	// that can display a popup.
	//
	// Parameters:
	//     pos - The mouse position to test, in screen coords.
	//     delay - [out] The number of ticks of delay before this popup should
	//             be displayed.
	//
	// Returns:
	//     A bool that is true if the coords are over a popup, and false if
	//     not.
	//
	virtual bool TestPopup(const wxPoint& pos, wxInt32& delay) const=0;

	//-----------------------------------------------------------------------//
	// Function: CreatePopup
	//
	// Populates the given sizer to create the popup.
	//
	// Parameters:
	//     window - The parent popup window.
	//     sizer - The sizer to populate.
	//
	virtual void CreatePopup(wxWindow* window, wxSizer* sizer) const=0;

	//-----------------------------------------------------------------------//
	// Function: GetThis
	//
	// Returns the window the popup is attached to.
	//
	// Returns:
	//     The window the popup is attached to.
	//
	virtual wxWindow* GetThis() const=0;

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~IPopup
	//
	// The IPopup destructor.
	//
	virtual ~IPopup();
};

//---------------------------- PROTOTYPES       -----------------------------//
inline IPopup::~IPopup() {}
