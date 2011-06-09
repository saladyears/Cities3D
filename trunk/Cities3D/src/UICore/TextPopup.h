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
#include "BasePopup.h"
#include "UIExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: TextPopup
// 
// Simple text popup implementation.
//
// Derived From:
//     <BasePopup>
//
// Project:
//     <Cities3D>
//
// Include:
//     TextPopup.h
//
class UI_EXPORT_SPEC TextPopup : public BasePopup
{
	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: TextPopup
	//
	// The TextPopup constructor.
	//
	// Parameters:
	//     thisWindow - The window this popup is attached to.
	//     text - The text to display in the popup.
	//
	TextPopup(wxWindow* thisWindow, const wxString& text);

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: SetText
	//
	// Sets the text.
	//
	// Parameters:
	//     text - The new text.
	//
	void SetText(const wxString& text) const
	{
		mText = text;
	}

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: CreatePopup
	//
	// Populates the given sizer to create the popup.
	//
	// Parameters:
	//     window - The parent popup window.
	//     sizer - The sizer to populate.
	//
	virtual void CreatePopup(wxWindow* window, wxSizer* sizer) const;

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// String: mText
	//
	// The text to display in the popup.
	//
	mutable wxString mText;
};

//---------------------------- PROTOTYPES       -----------------------------//

