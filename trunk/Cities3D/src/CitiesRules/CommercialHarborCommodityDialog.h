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
// Class: wxCommercialHarborCommodityDialog
// 
// Displays a dialog where the user can select one commodity to trade via the
// Commercial Harbor.
//
// Derived From:
//     <wxResoruceSelectionDialog>
//
// Project:
//     <Cities3D>
//
// Include:
//     CommercialHarborCommodityDialog.h
//
class wxCommercialHarborCommodityDialog : public wxResourceSelectionDialog
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxCommercialHarborCommodityDialog
	//
	// The wxCommercialHarborCommodityDialog constructor.
	//
	// Parameters:
	//     parent - The parent window of the dialog.
	//     game - The current <Game>.
	//
	wxCommercialHarborCommodityDialog(wxWindow *parent, 
		boost::weak_ptr<Game> game);
};

//---------------------------- PROTOTYPES       -----------------------------//

