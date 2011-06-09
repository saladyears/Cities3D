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
#include "CardListCtrl.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxStealProgressCardListCtrl
// 
// Displays stealable progress cards in a list.
//
// Derived From:
//     <wxStealProgressCardListCtrl>
//
// Project:
//     <CitiesRules>
//
// Include:
//     ProgressCardListCtrl.h
//
class wxStealProgressCardListCtrl : public wxCardListCtrl
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxStealProgressCardListCtrl
	// 
	// The wxStealProgressCardListCtrl constructor.  Creates the columns for 
	// the list control.
	//
	// Parameters:
	//     parent - The parent window.
	//     id - The message handling ID.
	//     game - The current <Game>.
	//     victim - The victim.
	//     pos - The list control position in window coordinates.
	//     size - The list control size.
	//
	wxStealProgressCardListCtrl(wxWindow* parent, wxWindowID id, 
		boost::weak_ptr<Game> game, wxInt32 victim,
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize);
};

//---------------------------- PROTOTYPES       -----------------------------//

