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
#include "RuleExtensionMixin.h"
#include "IUndoable.h"
#include "WallsObject.h"
#include "AggregatorObject.h"
#include "Controller.h"
#include "Utility.h"
#include "AnimationPlacement.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePlaceWalls
// 
// Places a city walls on the board at the given tile and corner for the current 
// player.  *Undoable*.
//
// Derived From:
//     <Rule>,
//     <IUndoable>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
// Rules Called:
//     <RuleUpdateNetworkUI>
//     <RuleAnimatePlacement>
//
// Transmits To:
//     eventGameObjects
//
class RulePlaceWalls 
	: public Rule
	, public IUndoable
	, public RuleExtensionMixin
{
public:
	virtual void Execute(const DataObject &object)
	{
		gameData<HashString>(shState) = shTurn;

		// Clear any dim tiles or objects.
		RULE.Execute(shRuleResetTileFlags, DataObject());
		RULE.Execute(shRuleResetObjectFlags, DataObject());

		wxASSERT(1 <= object.numItems());

		// Update the UI.
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stPlacesACityWalls));

		//first, determine the tile and corner being built on
		wxInt32 tile, corner;
		boost::tie(tile, corner) = Utility::decodeSel(object.read<wxInt32>());

		PlayerGame &playergame(playerGame());
		ColorType color = playergame.color();

		wxInt32 turn = gameData<wxInt32>(shTurn);

		// Now find the city this walls is underneath and wire them up.
		PlayerGame::CornerObjectArray cities = 
			playerGame().getCornerObjects(shCities);
		CornerObjectPtr city;
		PlayerGame::CornerObjectArray::iterator it, itEnd = cities.end();
		for(it = cities.begin(); it != itEnd; ++it)
		{
			if(true == (*it)->onTileCorner(
				CornerObject::TileCorner(tile, corner)))
			{
				city = (*it);
				break;
			}
		}
		wxASSERT(city);

		// Make sure the walls are at the bottom of whatever pile there
		// might be.
		CornerObjectPtr thisObject = city;
		CornerObjectPtr below = thisObject->below().lock();
		while(below)
		{
			thisObject = below;
			below = thisObject->below().lock();
		}

		//now create the walls object for this player and add it to their 
		//list
		CornerObjectPtr walls(CornerObjectPtr(new WallsObject(
			current(), turn, color, GetGame(), tile, corner, thisObject)));
		playergame.addCornerObject(shWalls, walls);

		thisObject->below(walls);

		//subtract one from the player's stock walls
		playerGameData<wxInt32>(shStockWalls) -= 1;

		// They are now allowed 2 more cards in hand.
		playerGameData<wxInt32>(shAllowedCards) += 2;

		// Play the walls sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_PLACE_CITY_WALLS));

		//send it to the view
		AggregatorObjectPtr aggregate(new AggregatorObject);
		aggregate->add(walls);

		Controller::get().Transmit(shEventGameObjects, aggregate);

		// Run an animation if needed.
		RULE.Execute(shRuleAnimatePlacement, DataObject(playergame.player(),
			GameObjectPtr(walls), color));

		// Execute any extensions to walls placement.
		ExecuteExtensions(object);

		// Adjust the resources used.
		DataObject input(0), output;
		RULE.Decide(shLogicCostWalls, input, output);
		RULE.Execute(shRulePurchase, output);

		// Adjust stats.
		RULE.Execute(shRulePurchaseItem, DataObject(current(), shWalls));

		// If it was free, tick it off.
		wxBOOL builtFree = FALSE;

		wxInt32 &free = playerGameData<wxInt32>(shFreeWalls);
		if(0 < free)
		{
			builtFree = TRUE;
			--free;
		}

		playerGameData<wxInt32>(shWallsFree) = builtFree;

		// Back to turn actions.
		RULE.Execute(shRuleRestartTurn, DataObject());
	}

	virtual void Unexecute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		//first, determine the tile and corner being built on
		wxInt32 tile, corner;
		boost::tie(tile, corner) = Utility::decodeSel(object.read<wxInt32>());

		PlayerGame &playergame(playerGame());

		wxInt32 turn = gameData<wxInt32>(shTurn);

		//now create the walls object for this player and remove it from 
		//their list
		CornerObjectPtr walls(CornerObjectPtr(new WallsObject(
			current(), turn, playergame.color(), GetGame(), tile, corner,
			CornerObjectPtr())));
		playergame.removeCornerObject(shWalls, walls);

		//add one to the player's stock wallss
		playerGameData<wxInt32>(shStockWalls) += 1;

		// They are now allowed 2 less cards in hand.
		playerGameData<wxInt32>(shAllowedCards) -= 2;

		// Update the UI.
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stUndoesACityWalls));

		if(TRUE == playerGameData<wxInt32>(shWallsFree))
		{
			wxInt32 &free = playerGameData<wxInt32>(shFreeWalls);
			++free;
		}
	}

	virtual bool CanUndo() const
	{
		//default to true for walls placements, allow
		//container rules to decide
		return true;
	}
};

IMPLEMENT_RULE(RulePlaceWalls, CITIES)
