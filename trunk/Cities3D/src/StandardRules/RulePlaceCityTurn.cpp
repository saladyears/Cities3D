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
#include "RuleExtensionMixin.h"
#include "RuleSetDatabase.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePlaceCityTurn
// 
// Places a city on the board during a player's turn.
//
// Derived From:
//     <Rule>
//     <RuleExtensionMixin>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// Rules Called:
//     <RulePlaceCity>
//     <RulePurchase>
//     <RuleRestartTurn>
//
// Logic Called:
//     <LogicCostCity>
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
class RulePlaceCityTurn 
	: public Rule
	, public IUndoable
	, public RuleExtensionMixin
{
public:
	virtual void Execute(const DataObject &object)
	{
		gameData<HashString>(shState) = shTurn;

		// Clear any dim tiles.
		RULE.Execute(shRuleResetTileFlags, DataObject());

		// Run the standard placement of the city.
		RULE.Execute(shRuleRemoveSettlement, object);
		ExecuteExtensions(object);
		RULE.Execute(shRulePlaceCity, object);

		// Adjust the resources used.
		DataObject input(0), output;
		RULE.Decide(shLogicCostCity, input, output);
		RULE.Execute(shRulePurchase, output);

		// If it was free, tick it off.
		wxBOOL builtFree = FALSE;

		wxInt32 &free = playerGameData<wxInt32>(shFreeCities);
		if(0 < free)
		{
			builtFree = TRUE;
			--free;
		}

		playerGameData<wxInt32>(shCityBuiltFree) = builtFree;

		// Back to turn actions.
		RULE.Execute(shRuleRestartTurn, DataObject());
	}

	virtual void Unexecute(const DataObject &)
	{
		// Let the undo chain handle this.

		if(TRUE == playerGameData<wxInt32>(shCityBuiltFree))
		{
			wxInt32 &free = playerGameData<wxInt32>(shFreeCities);
			++free;
		}
	}

	virtual bool CanUndo() const
	{
		return true;
	}
};

IMPLEMENT_RULE(RulePlaceCityTurn, STANDARD)
