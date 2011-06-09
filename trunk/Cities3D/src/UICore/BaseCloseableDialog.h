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
#include "BaseGameDialog.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxBaseCloseableDialog
// 
// A modeless dialog box with a close button in the upper-right corner.
//
// Derived From:
//     <wxBaseGameDialog>
//
// Project:
//     <UICore>
//
// Include:
//     BaseCloseableDialog.h
//
class UI_EXPORT_SPEC wxBaseCloseableDialog : public wxBaseGameDialog
{
	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxBaseCloseableDialog
	//
	// The wxBaseCloseableDialog constructor.
	//
	// Parameters:
	//     parent - The parent window of the dialog.
	//     sizer - The <ISizer> that will create the dialog layout. Must not
	//             be NULL.
	//     label - The caption of the dialog.
	//     config - The config section of the dialog in the <ConfigDatabase>.
	//
	wxBaseCloseableDialog(wxWindow *parent, const ISizerPtr &sizer, 
		const wxString &label, const wxString &config);

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
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Close
	//
	// Allows dialog-specific close handling.
	//
	virtual void Close()
	{
	}

	//-----------------------------------------------------------------------//
	// Group: Event Functions
	//

	void OnClose(wxCloseEvent& event);
};

//---------------------------- PROTOTYPES       -----------------------------//

