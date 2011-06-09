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
#include "Logic.h"
#include "RuleSetDatabase.h"
#include "SideObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicIsValidRoadPlacementStandard
// 
// Determines if a road placement is valid in a standard game.  A road 
// placement is valid if the player has another road in any of the four
// branch positions from the placement.
//
// Derived From:
//     <Logic>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// Mixin To:
//     <LogicIsValidRoadPlacement>
//
// Logic Called:
//     <LogicIsValidSidePlacement>
//
class LogicIsValidRoadPlacementStandard : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		const wxInt32 index = input.read<wxInt32>();
		const wxInt32 side = input.read<wxInt32>(1);

		RULE.Decide(shLogicIsValidSidePlacement, 
			DataObject(shRoads, index, side), output);
	}
};

IMPLEMENT_LOGIC_MIXIN(LogicIsValidRoadPlacementStandard, 
					  LogicIsValidRoadPlacement, STANDARD);
