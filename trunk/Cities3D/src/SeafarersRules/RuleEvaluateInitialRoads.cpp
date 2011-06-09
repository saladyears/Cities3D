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
#include "GameEngine.h"
#include "Controller.h"
#include "Utility.h"
#include "IRuleEngine.h"
#include "ConfigDatabase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleEvaluateInitialRoads
// 
// Determines if only a road, only a ship, or either a road or 
// a ship can be built from an initial placement.  If only a road
// or only a ship can be built, calls <RuleRequestInitialRoad> to
// immediately go into that placement.  If both can be built, sets the
// <Game> State to InitialEvaluate, calls <RuleUpdateMessageUI> with
// "place a road or ship", and updates the <BuildUI> via the
// eventBuildUI <Event>.  *Undoable*.
//
// Derived From:
//     <Rule>,
//     <IUndoable>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <Seafarers>
//
// Rules Called:
//     <RuleRequestInitialRoad>
//     <RuleUpdateMessageUI>
//
// Logic Called:
//     <LogicPlaceInitialRoad>
//     <LogicPlaceInitialShip>
//
// Game State:
//     InitialEvaluate
//
// Transmits To:
//      eventBuildUI
//
class RuleEvaluateInitialRoads 
: public Rule
, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
		//count up how many roads we can build from the location and
		//how many ships, if we can build both, we'll have to let the user
		//decide via the BuildUI
		wxInt32 roads = 0, ships = 0;

		wxInt32 thisTile, corner;
		boost::tie(thisTile, corner) = Utility::decodeSel(
			object.read<wxInt32>());

		//examine the three road locations to see if they are valid
		testPlacement(thisTile, corner, thisTile, corner, roads, ships);
		testPlacement(thisTile, ((corner + 5) % 6), thisTile, corner, roads, 
			ships);

		//opposite tile and side
		wxInt32 otherTile = tile<IntArray>(shSides, thisTile)[corner];
		if(-1 != otherTile)
		{
			testPlacement(otherTile, ((corner + 4) % 6), thisTile, corner, 
				roads, ships);
		}

		//if all we have are road possibilities, go right into that
		if( (0 == ships) &&
			(0 != roads))
		{
			gameData<wxInt32>(shInitialShip) = 0;
			RULE.Execute(shRuleRequestInitialRoad, object);
		}
		//same for ships
		else if( (0 == roads) &&
				 (0 != ships))
		{
			gameData<wxInt32>(shInitialShip) = 1;
			RULE.Execute(shRuleRequestInitialRoad, object);
		}
		else
		{
			// The current player is now in a blocking action.
			RULE.Execute(shRuleBeginBlockingAction, DataObject(current()));

			HashString &state = gameData<HashString>(shState);

			if(shInitialEvaluate != state)
			{
				//store off the tile/corner location and the old state
				gameData<wxInt32>(shInitialEvaluate) = 
					object.read<wxInt32>();
				gameData<HashString>(shInitialEvaluateState) = 
					gameData<HashString>(shState);

				state = shInitialEvaluate;
			}

			// Add 1 to their free roads so we don't get popup text.
			playerGameData<wxInt32>(shFreeRoads) = 1;

			RULE.Execute(shRuleUpdateMessageUI, DataObject(stPlaceARoadOrShip, 
                stWaitingPlaceARoadOrShip));

			Controller::get().Transmit(shEventBuildUI, 
				DataObject(true, GetGame()));
		}
	}

	virtual void Unexecute(const DataObject &)
	{
		Controller::get().Transmit(shEventBuildUI, 
			DataObject(false, GetGame()));
	}

	virtual bool CanUndo() const
	{
		return true;
	}

private:
	void testPlacement(wxInt32 thisTile, wxInt32 side, wxInt32 origTile, 
		wxInt32 origCorner, int &roads, int &ships)
	{
		DataObject input(std::make_pair(thisTile, side),
			std::make_pair(origTile, origCorner)), output;
		RULE.Decide(shLogicPlaceInitialRoad, input, output);
		
		if(true == output.read<bool>())
		{
			++roads;
		}

		output.reset();
		RULE.Decide(shLogicPlaceInitialShip, input, output);
		
		if(true == output.read<bool>())
		{
			++ships;
		}
	}
};

IMPLEMENT_RULE(RuleEvaluateInitialRoads, SEAFARERS)
