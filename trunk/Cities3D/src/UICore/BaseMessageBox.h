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
#include "BaseModalDialog.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
enum MessageBoxType
{
	MessageBoxOK			= 0,
	MessageBoxYesNo			,
};

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxBaseMessageBox
// 
// A basic message box for displaying modal messages.
//
// Derived From:
//     <wxBaseModalDialog>
//
// Project:
//     <UICore>
//
// Include:
//     BaseMessageBox.h
//
class UI_EXPORT_SPEC wxBaseMessageBox : public wxBaseModalDialog
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxBaseMessageBox
	//
	// The wxBaseMessageBox constructor.
	//
	// Parameters:
	//     parent - The parent window of the dialog.
	//     label - The caption of the dialog.
	//     text - The display text of the dialog.
	//     type - The dialog type.
	//
	wxBaseMessageBox(wxWindow *parent, const wxString& label, 
		const wxString& text, MessageBoxType type = MessageBoxOK);

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
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnYes
	//
	// Handles the Yes button being clicked.
	//
	void OnYes(wxCommandEvent&);

	//-----------------------------------------------------------------------//
	// Function: OnNo
	//
	// Handles the No button being clicked.
	//
	void OnNo(wxCommandEvent&);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Object: mType
	//
	// The type of message box this is.
	//
	MessageBoxType mType;
};

//---------------------------- PROTOTYPES       -----------------------------//

