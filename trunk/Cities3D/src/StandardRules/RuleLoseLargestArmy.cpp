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
#include "IUndoable.h"
#include "RuleSetDatabase.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleLoseLargestArmy
// 
// Removes Largest Army from the given player.
//
// Derived From:
//     <Rule>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <DevCards>
//
class RuleLoseLargestArmy : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxInt32 player = object.read<wxInt32>();

		gameData<wxInt32>(shLargestArmyPlayer) = -1;

		// Losing Largest Army subtracts two points from the player.
		RULE.Execute(shRuleAdjustPoints, DataObject(player, -2));

		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stLosesLargestArmy, 
            player));
	}
};

IMPLEMENT_RULE(RuleLoseLargestArmy, DEVCARDS)
