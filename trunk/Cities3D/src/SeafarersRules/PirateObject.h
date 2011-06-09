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
#include "HexObject.h"
#include "RuleSetDatabase.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//
class PirateObject : public HexObject
{
	DECLARE_GAME_OBJECT(PirateObject);

public:
	static const wxInt32 PIRATE_ID = INT_MAX;

	PirateObject(const wxInt32 tile)
	: HexObject(tile)
	{
		id(PIRATE_ID);
	}

	PirateObject() : HexObject(0) {}
};

//---------------------------- PROTOTYPES       -----------------------------//
IMPLEMENT_GAME_OBJECT(PirateObject, SEAFARERS);
