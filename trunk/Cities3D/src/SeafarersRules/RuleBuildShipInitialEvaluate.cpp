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
#include "GameEngine.h"
#include "Controller.h"
#include "IRuleEngine.h"
#include "ConfigDatabase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleBuildShipInitialEvaluate
// 
// Handles a click on the ship <BuildUI> in the <Game> 
// State of InitialEvaluate.  Sets the InitialShip data field
// in the <Game> data to 1 to indicate that a ship is being placed.
// Calls <RuleRequestInitialRoad> to prepare the placements on the
// board for the ship.  Transmits to the eventBuildUI <Event> to
// turn off the <BuildUIs>. 
//
// Derived From:
//     <Rule>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <Seafarers>
//
// Mixin To:
//     <RuleBuildShip>
//
// Rules Called:
//     <RuleRequestInitialRoad>
//
// Transmits To:
//     eventBuildUI
//
class RuleBuildShipInitialEvaluate : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		//retrieve the corner of the last corner object placed
		wxInt32 tileCorner = gameData<wxInt32>(shInitialEvaluate);
		gameData<HashString>(shState) = 
			gameData<HashString>(shInitialEvaluateState);

		//no longer need those data bite
		eraseGameData<wxInt32>(shInitialEvaluate);
		eraseGameData<HashString>(shInitialEvaluateState);

		// Restore popup text.
		playerGameData<wxInt32>(shFreeRoads) = 0;

		gameData<wxInt32>(shInitialShip) = 1;
		RULE.Execute(shRuleRequestInitialRoad, DataObject(tileCorner));

		// Undo is not allowed for this.
		RULE.Execute(shRuleBreakUndo, DataObject());

		Controller::get().Transmit(shEventBuildUI, 
			DataObject(false, GetGame()));
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RuleBuildShipInitialEvaluate, InitialEvaluate, 
						   RuleBuildShip, SEAFARERS)
