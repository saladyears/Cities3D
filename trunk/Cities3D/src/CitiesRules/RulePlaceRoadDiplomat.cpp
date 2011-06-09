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
#include "Rule.h"
#include "RuleSetDatabase.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePlaceRoadDiplomat
// 
// Places a road on the board during a player's turn.  *Undoable*.
//
// Derived From:
//     <Rule>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// Rules Called:
//     <RulePlaceRoad>
//     <RulePurchase>
//     <RuleRestartDiplomat>
//
// Logic Called:
//     <LogicCostRoad>
//
// Selection Rule:
//     <>
//
// Game State:
//
//
// Transmits To:
//
//
class RulePlaceRoadDiplomat : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		gameData<HashString>(shState) = shTurn;

		const HashString& type = gameData<HashString>(shDiplomatType);
		HashString rule = HashString::Format(shRulePlaceString.c_str(), 
            type.c_str());

		// Clear any dim tiles.
		RULE.Execute(shRuleResetObjectFlags, DataObject());
		RULE.Execute(shRuleResetTileFlags, DataObject());

		// Run the standard placement of the road (or ship).
		RULE.Execute(rule, object);

		// Longest Road Check.
		RULE.Execute(shRuleLongestRoadCheck, DataObject(true));

		// Back to turn actions.
		RULE.Execute(shRuleRestartTurn, DataObject());
	}
};

IMPLEMENT_RULE(RulePlaceRoadDiplomat, CITIES)
