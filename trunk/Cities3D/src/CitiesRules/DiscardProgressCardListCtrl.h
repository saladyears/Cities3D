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
// Class: wxDiscardProgressCardListCtrl
// 
// Displays discardable progress cards in a list.
//
// Derived From:
//     <wxDiscardProgressCardListCtrl>
//
// Project:
//     <CitiesRules>
//
// Include:
//     DiscardProgressCardListCtrl.h
//
class wxDiscardProgressCardListCtrl : public wxCardListCtrl
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxDiscardProgressCardListCtrl
	// 
	// The wxDiscardProgressCardListCtrl constructor.  Creates the columns for 
	// the list control.
	//
	// Parameters:
	//     parent - The parent window.
	//     id - The message handling ID.
	//     game - The current <Game>.
	//     player - The player.
	//     pos - The list control position in window coordinates.
	//     size - The list control size.
	//
	wxDiscardProgressCardListCtrl(wxWindow* parent, wxWindowID id, 
		boost::weak_ptr<Game> game, wxInt32 player,
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize);
};

//---------------------------- PROTOTYPES       -----------------------------//

