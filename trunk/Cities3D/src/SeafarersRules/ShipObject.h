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
#include "SideObject.h"
#include "RuleSetDatabase.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//
class ShipObject : public SideObject
{
	DECLARE_GAME_OBJECT(ShipObject);

public:
	ShipObject(wxInt32 owner, wxInt32 turn, const ColorType color, 
		const GamePtr &game, const wxInt32 tile, const wxInt32 side)
	: SideObject(owner, turn, color, game, tile, side)
	{
	}

	ShipObject() : SideObject(-1, -1, CR_WHITE, GamePtr(), 0, 0) {}

	virtual Vector uicoords() const
	{
		return Vector(0.0f, -0.8333f, 0.0f);
	}
};

//---------------------------- PROTOTYPES       -----------------------------//
IMPLEMENT_GAME_OBJECT(ShipObject, SEAFARERS);
