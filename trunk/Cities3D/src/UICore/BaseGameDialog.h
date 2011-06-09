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
#include "BaseDialog.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxBaseGameDialog
// 
// The base class for all in game dialog windows that must shut themselves down
// when a global shutdown event occurs and refresh the main GL window when
// destroyed.
//
// Derived From:
//     <wxBaseDialog>
//
// Project:
//     <UICore>
//
// Include:
//     BaseGameDialog.h
//
class UI_EXPORT_SPEC wxBaseGameDialog : public wxBaseDialog
{
	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxBaseGameDialog
	//
	// The wxBaseGameDialog constructor.
	//
	// Parameters:
	//     parent - The parent window of the dialog.
	//     sizer - The <ISizer> that will create the dialog layout. Must not
	//             be NULL.
	//     label - The caption of the dialog.
	//     config - The config section of the dialog in the <ConfigDatabase>.
	//     style - The style of the dialog.
	//
	wxBaseGameDialog(wxWindow *parent, const ISizerPtr &sizer, 
		const wxString &label, const wxString &config, long style = 0);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxBaseGameDialog
	//
	// The wxBaseGameDialog destructor.
	//
	virtual ~wxBaseGameDialog();

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnShutdownUI
	//
	// Shuts down the dialog when a global shutdown occurs.
	//
	void OnShutdownUI(wxInt32);
};

//---------------------------- PROTOTYPES       -----------------------------//

