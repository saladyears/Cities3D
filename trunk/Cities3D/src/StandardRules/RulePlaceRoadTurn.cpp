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
// Class: RulePlaceRoadTurn
// 
// Places a road on the board during a player's turn.  *Undoable*.
//
// Derived From:
//     <Rule>
//     <IUndoable>
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
//     <RuleRestartTurn>
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
class RulePlaceRoadTurn 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
		gameData<HashString>(shState) = shTurn;

		// This trick allows road lengths to be recalculated properly after
		// an undo.
		RULE.Execute(shRuleLongestRoadCheck, DataObject(false));

		// Clear any dim tiles.
		RULE.Execute(shRuleResetObjectFlags, DataObject());
		RULE.Execute(shRuleResetTileFlags, DataObject());

		// Run the standard placement of the road.
		RULE.Execute(shRulePlaceRoad, object);

		// Adjust the resources used.
		DataObject input(0), output;
		RULE.Decide(shLogicCostRoad, input, output);
		RULE.Execute(shRulePurchase, output);

		// If it was free, tick it off.
		wxBOOL builtFree = FALSE;

		wxInt32 &free = playerGameData<wxInt32>(shFreeRoads);
		if(0 < free)
		{
			builtFree = TRUE;
			--free;
		}

		playerGameData<wxInt32>(shRoadBuiltFree) = builtFree;

		// Longest Road Check.
		RULE.Execute(shRuleLongestRoadCheck, DataObject(true));

		// Back to turn actions.
		RULE.Execute(shRuleRestartTurn, DataObject());
	}

	virtual void Unexecute(const DataObject &)
	{
		if(TRUE == playerGameData<wxInt32>(shRoadBuiltFree))
		{
			wxInt32 &free = playerGameData<wxInt32>(shFreeRoads);
			++free;
		}
	}

	virtual bool CanUndo() const
	{
		// The Longest Road check will automatically disallow undoes if it
		// alters anything.
		return true;
	}
};

IMPLEMENT_RULE(RulePlaceRoadTurn, STANDARD)
