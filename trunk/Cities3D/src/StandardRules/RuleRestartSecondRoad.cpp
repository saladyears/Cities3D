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
#include "CornerObject.h"
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
// Class: RuleRestartSecondRoad
// 
// Restarts the <Game> from the State of InitialRoad.
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
// Mixin To:
//     <RuleRestartGame>
//
// Rules Called:
//     <RuleRequestInitialRoad>
//
// Transmits To:
//     eventControlsUI
//
class RuleRestartSecondRoad : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		//show the controls UI
		Controller::get().Transmit(shEventControlsUI, DataObject(true));

		//retrieve that last corner object built for this player,
		//which will be the starting location for their road
		const PlayerGame::CornerObjectArray &array = 
			playerGame().getCornerObjects(shSettlements);

		//now get the last one
		const CornerObjectPtr &cornerObject = *(--array.end());

		wxInt32 tile, corner;
		boost::tie(tile, corner) = cornerObject->tile1();

		RULE.Execute(shRuleRequestInitialRoad, 
			DataObject(Utility::encodeSel(tile, corner)));
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RuleRestartSecondRoad, SecondRoad, RuleRestartGame, 
						   STANDARD);
