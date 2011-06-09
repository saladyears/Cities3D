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
// Class: wxBaseModalDialog
// 
// A common base class for modal dialog boxes.
//
// Derived From:
//     <wxBaseDialog>
//
// Project:
//     <UICore>
//
// Include:
//     BaseDialog.h
//
class UI_EXPORT_SPEC wxBaseModalDialog : public wxBaseDialog
{
	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxBaseModalDialog
	//
	// The wxBaseModalDialog constructor.  Creates the dialog and
	// then calls the CreateSizers function of the given <ISizer> pointer,
	// which creates the specific layout for that dialog window.
	//
	// Parameters:
	//     parent - The parent window of the dialog.
	//     sizer - The <ISizer> that will create the dialog layout. Must 
	//             not be NULL.
	//     label - The caption of the dialog.
	//     config - The config section of the dialog in the <ConfigDatabase>.
	//     style - Any style on the dialog.
	//
	wxBaseModalDialog(wxWindow *parent, const ISizerPtr &sizer, 
		const wxString &label, const wxString &config, 
		long style = wxDEFAULT_DIALOG_STYLE);
	
	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxBaseModalDialog
	//
	virtual ~wxBaseModalDialog();

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Ok
	//
	// Allows derived classes to handle the OK event.  Called
	// when the user clicks on the OK button.  The default
	// implementation ends the dialog with a wxID_OK return code.
	//
	virtual void Ok();

	//-----------------------------------------------------------------------//
	// Function: Cancel
	//
	// Allows derived classes to handle the Cancel event.  Called
	// when the user clicks on the Cancel button.  The default
	// implementation ends the dialog with a wxID_CANCEL return code.
	//
	virtual void Cancel();

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
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnKeyDown
	//
	// Looks for the ESC button or the Return key to be pressed.  It is virtual
	// so that any dialog may override the default handling if desired (i.e., 
	// not calling Cancel on Escape, or OK on Enter).
	//
	// Parameters:
	//     event - The key press event.
	//
	virtual void OnKeyDown(const wxKeyEvent &event);

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnOk
	//
	// Handles the OK button being clicked.  Immediately calls the
	// Ok function.
	//
	// Parameters:
	//     event - The button click event.
	//
	void OnOk(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnCancel
	//
	// Handles the Cancel button being clicked.  Immediately calls the
	// Cancel function.
	//
	// Parameters:
	//     event - The button click event.
	//
	void OnCancel(wxCommandEvent &event);
};

//---------------------------- PROTOTYPES       -----------------------------//
