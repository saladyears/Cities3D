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
#include "CornerObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicIsValidSettlementPlacementStandard
// 
// Determines if a settlement placement is valid in a standard game.  A 
// placement is valid if the player has two roads connected from the placement
// with no cities or settlements on the inbetween corner.
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
//     <LogicIsValidSettlementPlacement>
//
class LogicIsValidSettlementPlacementStandard : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		RULE.Decide(shLogicIsCornerSettleable, DataObject(
			input.read<CornerObject::TileCorner>(), shRoads), 
			output);
	}
};

IMPLEMENT_LOGIC_MIXIN(LogicIsValidSettlementPlacementStandard, 
					  LogicIsValidSettlementPlacement, STANDARD);
