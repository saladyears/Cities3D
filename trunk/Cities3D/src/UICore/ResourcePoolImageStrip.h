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
#include "InteractiveImageStrip.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//
class IResourceLogic;
class Game;

//---------------------------------------------------------------------------//
// Class: wxResourcePoolImageStrip
//
// A <wxImageStrip> for displaying a pool of resources for the user to choose
// from.
//
// Derived From:
//     <wxImageStrip>
//
// Project:
//     <Cities3D>
//
// Include:
//     ResourcePoolImageStrip.h
//
class UI_EXPORT_SPEC wxResourcePoolImageStrip : public wxInteractiveImageStrip
{
public:
	wxResourcePoolImageStrip(wxWindow* parent, wxWindowID id, 
		boost::weak_ptr<Game> game, IResourceLogic* logic, bool any = false);
};

//---------------------------- PROTOTYPES       -----------------------------//

