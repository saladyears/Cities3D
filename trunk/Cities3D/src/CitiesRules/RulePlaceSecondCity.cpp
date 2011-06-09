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
#include "IUndoable.h"
#include "GameEngine.h"
#include "Controller.h"
#include "Utility.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePlaceSecondCity
// 
// Calls <RulePlaceCity> with the given location to place a city 
// on the board.  This occurs during the second round of initial 
// placements in Cities & Knights.  Sets the <Game> State to 
// SecondCityRoad to prepare for placing a road off the city.
// Calls <RuleInitialResources> to increment the resources for
// the player based on their city placement.  *Undoable*.
//
// Derived From:
//     <Rule>
//     <IUndoable>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
// Rules Called:
//     <RulePlaceCity>,
//     <RuleInitialResources>
//
// Mixin To:
//     <RulePlaceInitialPlacement>
//
// Game State:
//     SecondCityRoad
//
class RulePlaceSecondCity 
: public Rule
, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
		RULE.Execute(shRulePlaceCity, object);

		//we're now on to placing the second road
		gameData<HashString>(shState) = shSecondCityRoad;

		//add in the inital resources they get
		RULE.Execute(shRuleInitialResources, object);

		//trust the rule chain to get us to the next rule
	}

	virtual void Unexecute(const DataObject &)
	{
		//undo is done in reverse order automatically

		gameData<HashString>(shState) = shSecondCity;
	}

	virtual bool CanUndo() const
	{
		//initial cities are *always* undoable
		return true;
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RulePlaceSecondCity, SecondCity, RulePlaceInitialPlacement, CITIES);
