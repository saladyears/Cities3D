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
#include "BuildData.h"
#include "RuleSetDatabase.h"
#include "DrawShipObject.h"
#include "ShipObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//---------------------------- STATICS          -----------------------------//
//seafarers build items
IMPLEMENT_BUILD_OPENGL_DATA(SEAFARERS, 0, 5, LogicCanBuildShip, RuleBuildShip, 
							ShipObject, DrawShipObject, ship96, no_mask, 
							Trade, stBuildAShip, LogicCostShip, BuildShip);

IMPLEMENT_BUILD_OPENGL_DATA(SEAFARERS, 1, 5, LogicCanMoveShip, RuleMoveShip, 
							ShipObject, DrawShipObject, ship96, no_mask, 
							Move, stMoveAShip, LogicCostNothing, MoveShip);

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
