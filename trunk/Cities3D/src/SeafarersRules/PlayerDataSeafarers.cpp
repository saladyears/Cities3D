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
#include "stdwx.h"
#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//
#include "PlayerData.h"
#include "RuleSetDatabase.h"
#include "Game.h"
#include "ShipObject.h"
#include "DrawShipObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//---------------------------- STATICS          -----------------------------//
namespace
{
	wxInt32 updateAmount(GamePtr game, const wxInt32 index, 
		const HashString &value)
	{
		return game->playerGame(index).read<wxInt32>(value);
	}
};

//stock data
IMPLEMENT_PLAYER_OPENGL_DATA(SEAFARERS, PlayerStock, 
							 boost::bind(updateAmount, _1, _2, 
							 shStockShips), PlayerColorFunc(), -1, 
							 ShipObject, DrawShipObject, ship32, no_mask,
							 stShips);

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
