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
#include "Controller.h"
#include "IRuleEngine.h"
#include "CornerSelectionObject.h"
#include "CornerObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRestartTurnDeserterPlace
// 
// Restarts the game from a player placing a freshly deserted knight.
//
// Derived From:
//     <Rule>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
// Mixin To:
//     <RuleRestartGame>
//
class RuleRestartTurnDeserterPlace : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
        // The current player is now in a blocking action.
        RULE.Execute(shRuleBeginBlockingAction, DataObject(current()));

		RULE.Execute(shRuleUpdateMessageUI, DataObject(stPlaceTheDeserter, 
            stWaitingPlaceTheDeserter));

		Controller::get().Transmit(shEventControlsUI, DataObject(false));
		Controller::get().Transmit(shEventBuildUI, 
				DataObject(false, GetGame()));
	
		DataObject input(current()), output;
		RULE.Decide(shLogicCanPlaceKnight, input, output);

		// If they have positions to place, then see if they have the knights
		// in stock to place.
		typedef std::pair<wxInt32, wxInt32> TileCorner;
		typedef std::set<TileCorner> TileCornerSet;
		const TileCornerSet& corners = output.read<TileCornerSet>();

		RULE.Execute(shRuleBuildKnightCommon, DataObject(corners, 
			shRulePlaceDeserter));
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RuleRestartTurnDeserterPlace, TurnDeserterPlace, 
						   RuleRestartGame, CITIES)
