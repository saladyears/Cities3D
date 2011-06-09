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
#include "ResourceSelectionDialog.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxWeddingDialog
// 
// Displays a dialog where the user can select two resources or commodities to 
// give to the Wedding.
//
// Derived From:
//     <wxResourceSelectionDialog>
//
// Project:
//     <Cities3D>
//
// Include:
//     MerchantFleetDialog.h
//
class wxWeddingDialog : public wxResourceSelectionDialog
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxWeddingDialog
	//
	// The wxWeddingDialog constructor.
	//
	// Parameters:
	//     parent - The parent window of the dialog.
	//     index - The index of the player giving cards.
	//     game - The current <Game>.
	//
	wxWeddingDialog(wxWindow *parent, wxInt32 index, 
		boost::weak_ptr<Game> game);
};

//---------------------------- PROTOTYPES       -----------------------------//

