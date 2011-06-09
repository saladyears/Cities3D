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
#include "DrawRoadObject.h"
#include "DrawCityObject.h"
#include "DrawSettlementObject.h"
#include "RoadObject.h"
#include "CityObject.h"
#include "SettlementObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//---------------------------- STATICS          -----------------------------//
//standard build items
IMPLEMENT_BUILD_OPENGL_DATA(STANDARD, 0, 10, LogicCanBuildRoad, RuleBuildRoad, 
							RoadObject, DrawRoadObject, road96, no_mask, 
							Trade, stBuildARoad, LogicCostRoad, BuildRoad);
IMPLEMENT_BUILD_OPENGL_DATA(STANDARD, 0, 20, LogicCanBuildSettlement, 
							RuleBuildSettlement, SettlementObject, 
							DrawSettlementObject, settlement96, no_mask, 
							Trade, stBuildASettlement, 
							LogicCostSettlement, BuildSettlement);
IMPLEMENT_BUILD_OPENGL_DATA(STANDARD, 0, 30, LogicCanBuildCity, RuleBuildCity, 
							CityObject, DrawCityObject, city96, no_mask, 
							Trade, stBuildACity, LogicCostCity, BuildCity);

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
