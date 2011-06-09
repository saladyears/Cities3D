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

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicPlaceFirstRoad
// 
// Provides <RulePlaceInitialRoad> with the placement rule
// <RulePlaceRoad>, the advancement rule <RuleIncreaseInitialPlayer>,
// and the <Game> State FirstSettlement.
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
//     <RulePlaceInitialRoad>
//
class LogicPlaceFirstRoad : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(shRulePlaceRoad, shRuleIncreaseInitialPlayer,
			shFirstSettlement);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicPlaceFirstRoad, FirstRoad, 
							RulePlaceInitialRoad, STANDARD);
