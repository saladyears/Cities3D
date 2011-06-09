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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRestartTurnShipMovePlacement
// 
// Restarts the game from a player selecting a location to move their ship
// during their turn.
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
//     <RuleRestartGame>
//
// Rules Called:
//     <RuleBuildShipTurn>
//
// Selection Rule:
//     <RuleSelectShipMove>
//
// Transmits To:
//     eventBuildUI
//     eventSelectionObject
//
class RuleRestartTurnShipMovePlacement : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// Get all available placmements and remove the one where they just
		// took away the ship.
		DataObject input(0), output;
		RULE.Decide(shLogicCanPlaceShip, input, output);

		TileSideSet sides = output.read<TileSideSet>();
		wxASSERT(false == sides.empty());

		wxInt32 thisTile = gameData<wxInt32>(shShipMoveTile);
		wxInt32 thisSide = gameData<wxInt32>(shShipMoveSide);

		wxInt32 otherTile = tile<IntArray>(shSides, thisTile)[thisSide];
		wxInt32 otherSide = (thisSide + 3) % 6;

		TileSide tile1 = TileSide(thisTile, thisSide);
		TileSide tile2 = TileSide(otherTile, otherSide);

		TileSideSet::iterator it = sides.find(tile1);
		if(sides.end() != it)
		{
			sides.erase(it);
		}

		it = sides.find(tile2);
		if(sides.end() != it)
		{
			sides.erase(it);
		}

		wxASSERT(false == sides.empty());

		HashString rule = shRulePlaceShipMove;
		RULE.Execute(shRuleBuildShipCommonPlaces, DataObject(sides, rule));

		// Make sure eveything is hidden.
		Controller::get().Transmit(shEventBuildUI, 
			DataObject(false, GetGame()));
		Controller::get().Transmit(shEventControlsUI, DataObject(false));

		// Update the UI text.
		RULE.Execute(shRuleUpdateMessageUI, DataObject(stMoveYourShip, 
            stWaitingToMoveAShip));
	}

private:
	typedef std::pair<wxInt32, wxInt32> TileSide;
	typedef std::set<TileSide> TileSideSet;
};

IMPLEMENT_RULE_KEYED_MIXIN(RuleRestartTurnShipMovePlacement, 
						   TurnShipMovePlacement, RuleRestartGame, SEAFARERS)
