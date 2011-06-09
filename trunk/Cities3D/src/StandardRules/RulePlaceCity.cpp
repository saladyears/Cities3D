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
#include "IUndoable.h"
#include "RuleExtensionMixin.h"
#include "CityObject.h"
#include "AggregatorObject.h"
#include "Controller.h"
#include "Utility.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePlaceCity
// 
// Places a city on the board at the given tile and corner 
// for the current player.  Adds a new <CityObject> to the 
// current player's <PlayerGame> data. Increments the player's 
// points by 2. Subtracts one from the current player's 
// StockCities.  Plays the city sound.  Sends the new <CityObject>
// up to the View via the eventGameObjects <Event>. *Undoable*.
//
// Derived From:
//     <Rule>,
//     <IUndoable>
//     <RuleExtensionMixin>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// Rules Called:
//     <RulePlaySound>,
//     <RuleUpdateNetworkUI>
//
// Transmits To:
//     eventGameObjects
//
class RulePlaceCity 
: public Rule
, public IUndoable
, public RuleExtensionMixin
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		//first, determine the tile and corner being built on
		wxInt32 tile, corner;
		boost::tie(tile, corner) = Utility::decodeSel(object.read<wxInt32>());

		PlayerGame &playergame(playerGame());
		ColorType color = playergame.color();

		wxInt32 turn = gameData<wxInt32>(shTurn);

		//now create the city object for this player and add it to their list
		CornerObjectPtr city(CornerObjectPtr(new CityObject(current(), turn, 
			color, GetGame(), tile, corner)));
		playergame.addCornerObject(shCities, city);

		//subtract one from the player's stock cities
		playerGameData<wxInt32>(shStockCities) -= 1;

		//play the correct sound
		RULE.Execute(shRulePlaySound, DataObject(SOUND_PLACE_CITY));

		//send it to the view
		AggregatorObjectPtr aggregate(new AggregatorObject);
		aggregate->add(city);

		Controller::get().Transmit(shEventGameObjects, aggregate);

		RULE.Execute(shRuleAnimatePlacement, DataObject(playergame.player(),
			GameObjectPtr(city), color));

		// Adjust stats.
		RULE.Execute(shRulePurchaseItem, DataObject(current(), shCity));

		// Update the UI.
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stPlacesACity));

		// Add two points to the player's score.
		RULE.Execute(shRuleAdjustPoints, DataObject(current(), 2));

		ExecuteExtensions(object);
	}

	virtual void Unexecute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		//first, determine the tile and corner being built on
		wxInt32 tile, corner;
		boost::tie(tile, corner) = 
			Utility::decodeSel(object.read<wxInt32>());

		PlayerGame &playergame(playerGame());

		wxInt32 turn = gameData<wxInt32>(shTurn);

		//now create the settlement object for this player and remove it from their 
		//list
		CornerObjectPtr city(CornerObjectPtr(
			new CityObject(current(), turn, playergame.color(), GetGame(), 
			tile, corner)));
		playergame.removeCornerObject(shCities, city);

		//add one to the player's stock settlements
		playerGameData<wxInt32>(shStockCities) += 1;

		// Update the UI.
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stUndoesACity));

		// Subtract two points from the player's score.
		RULE.Execute(shRuleAdjustPoints, DataObject(current(), -2));
	}

	virtual bool CanUndo() const
	{
		//default to true for city placements, allow container rules to decide
		return true;
	}
};

IMPLEMENT_RULE(RulePlaceCity, STANDARD)
