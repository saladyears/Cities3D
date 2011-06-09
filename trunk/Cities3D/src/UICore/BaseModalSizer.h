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
#include "ISizer.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: BaseModalSizer
// 
// An base implementation of the <ISizer> interface that provides
// OK and Cancel <wxBaseButtons> for a modal dialog.
//
// Derived From:
//     <ISizer>
//
// Project:
//     <Cities3D>
//
// Include:
//     BaseModalSizer.h
//
class UI_EXPORT_SPEC BaseModalSizer : public ISizer
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: BaseModalSizer
	//
	// The BaseModalSizer constructor.  The given wxWindow must
	// be the window to be sized by BaseModalSizer.
	//
	// Parameters:
	//     window - The wxWindow to be sized in CreateSizers.
	//
	BaseModalSizer(wxWindow *window) : ISizer(window) {}

	//-----------------------------------------------------------------------//
	// Function: CreateLayout
	//
	// Creates the modal dialog layout.
	//
	// Parameters:
	//     sizer - The top level wxBoxSizer.  All subsequent
	//              sizers should be added as children to sizer.
	//
	virtual void CreateLayout(wxBoxSizer *sizer);

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//
	
	//-----------------------------------------------------------------------//
	// Function: CreateSizers
	//
	// Creates the dialog body.  Must be supplied by the derived class.
	//
	virtual void CreateSizers(wxBoxSizer *pSizer)=0;

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: CreateButtons
	//
	// Creates any control buttons.  Derived classes must
	// supply this function to provide any OK or Cancel buttons
	// required.
	//
	// Parameters:
	//     pSizer - The top level wxBoxSizer.  All subsequent
	//              sizers should be added as children to pSizer.
	//
	virtual void CreateButtons(wxBoxSizer *pSizer);
};

//---------------------------- PROTOTYPES       -----------------------------//

