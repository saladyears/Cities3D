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
#include "BaseGameDialog.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class TradeLogic;
typedef boost::shared_ptr<TradeLogic> TradeLogicPtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxAlchemistDialog
// 
// Special dialog where a user selects from two <wxImageLists> to pick the
// next dice roll.
//
// Derived From:
//     <wxBaseGameDialog>
//
// Project:
//     <Cities3D>
//
// Include:
//     AlchemistDialog.h
//
class wxAlchemistDialog : public wxBaseGameDialog
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxAlchemistDialog
	//
	// The wxAlchemistDialog constructor.
	//
	// Parameters:
	//     parent - The parent window of the dialog.
	//
	wxAlchemistDialog(wxWindow *parent);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxAlchemistDialog
	//
	// The wxAlchemistDialog destructor.
	//
	virtual ~wxAlchemistDialog();

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
	// Function: OnButton
	//
	// Handles a click on the button, which executes the selection.
	//
	// Parameters:
	//     event - The button click event.
	//
	void OnButton(wxCommandEvent &event);


	TradeLogicPtr mLogic;
};

//---------------------------- PROTOTYPES       -----------------------------//

