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
class ISizer;
typedef boost::shared_ptr<ISizer> ISizerPtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxBaseDialog
// 
// A common base class for dialog boxes.  The wxBaseDialog class provides
// a common look and feel to all classes derived from it.  This allows
// a consistent UI across the entire application and even across different
// OS platforms.  By default, wxBaseDialog provides a black background,
// with bitmapped title and text, and a resizable, custom-drawn frame.
//
// Derived From:
//     <wxCustomFrame>
//
// Project:
//     <UICore>
//
// Include:
//     BaseDialog.h
//
class UI_EXPORT_SPEC wxBaseDialog : public wxDialog
{
	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxBaseDialog
	//
	// The wxBaseDialog constructor.  Creates the dialog and
	// then calls the CreateLayout function of the given <ISizer> pointer,
	// which creates the specific layout for that dialog window.
	//
	// Parameters:
	//     parent - The parent window of the dialog.
	//     sizer - The <ISizer> that will create the dialog layout. Must not
	//             be NULL.
	//     label - The caption of the dialog.
	//     config - The config section of the dialog in the <ConfigDatabase>.
	//     style - The dialog style.
	//
	wxBaseDialog(wxWindow *parent, const ISizerPtr &sizer, 
		const wxString &label, const wxString &config, long style);
	
	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxBaseDialog
	//
	virtual ~wxBaseDialog();

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Size
	//
	// Allows derived classes to handle the Size event.
	//
	// Parameters:
	//     event - The size event.
	//
	virtual void Size(wxSizeEvent &)
	{
	}

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
	// Function: OnSize
	//
	// Handles the size event.
	// 
	// Parameters:
	//     event - The size event.
	//
	void OnSize(wxSizeEvent &event);
	
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// String: mConfig
	//
	// The section in the <ConfigDatabase> that stores this dialog's 
	// information.
	//
	wxString mConfig;
};

//---------------------------- PROTOTYPES       -----------------------------//
