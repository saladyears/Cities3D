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
#include "MetropolisObject.h"
#include "AggregatorObject.h"
#include "Vector.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleSelectMetropolisCity
// 
// Handles a player selecting a city to place a metropolis on.
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
class RuleSelectMetropolisCity : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
        // The current player has ended a blocking action.
        RULE.Execute(shRuleEndBlockingAction, DataObject(current()));

		wxInt32 id = object.read<wxInt32>();
		
		const PlayerGame::CornerObjectArray& cities = 
			playerGame().getCornerObjects(shCities);

		CornerObjectPtr city;
		PlayerGame::CornerObjectArray::const_iterator it, itEnd = cities.end();
		for(it = cities.begin(); it != itEnd; ++it)
		{
			if((*it)->id() == id)
			{
				city = (*it);
				break;
			}
		}
		wxASSERT(city);

		// Play the metropolis sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_PLACE_METROPOLIS));

		// Now that we have the location, create the metropolis object and
		// place it.
		const HashString& type = gameData<HashString>(shMetropolisType);
		MetropolisObject* metro = NULL;

		wxInt32 owner = current();
		wxInt32 turn = gameData<wxInt32>(shTurn);
		wxInt32 tile = city->tile1().first;
		wxInt32 corner = city->tile1().second;

		MetropolisObject::Type metroType = MetropolisObject::Trade;

		// Create the right kind.
		if(shTrade == type)
		{
			metro = new TradeMetropolisObject(owner, turn, GetGame(), tile, 
				corner);
			metroType = MetropolisObject::Trade;
		}
		else if(shPolitics == type)
		{
			metro = new PoliticsMetropolisObject(owner, turn, GetGame(), tile, 
				corner);
			metroType = MetropolisObject::Politics;
		}
		else if(shScience == type)
		{
			metro = new ScienceMetropolisObject(owner, turn, GetGame(), tile, 
				corner);
			metroType = MetropolisObject::Science;
		}
		wxASSERT(NULL != metro);

		// Add the object to the player's data and board.
		PlayerGame &playergame(playerGame());
		ColorType color = playergame.color();

		CornerObjectPtr metropolis(metro);
		playergame.addCornerObject(shMetropolis, metropolis);

		// Adjust the location of the metropolis to offset it a little from the
		// city.
		Vector coords = metro->coords();
		coords.z -= 0.75;
		coords.y = city->coords().y;
		metro->coords(coords);

		AggregatorObjectPtr aggregate(new AggregatorObject);
		aggregate->add(metropolis);

		Controller::get().Transmit(shEventGameObjects, aggregate);

		// The city is now linked to the metropolis.
		city->above(metropolis);
		metropolis->below(city);

		RULE.Execute(shRuleAnimatePlacement, DataObject(playergame.player(),
			GameObjectPtr(metropolis), color));

		HashString playerType = HashString::Format(shStringPlayer.c_str(), 
            type.c_str());

		DataObject input(type), output;
		RULE.Decide(shLogicUpgradeName, input, output);
		const wxString& name = output.read<wxString>();

		// See if anyone loses the metropolis.
		wxInt32 playerIndex = gameData<wxInt32>(playerType);
		
		gameData<wxInt32>(playerType) = owner;
		
		if(-1 != playerIndex)
		{
			wxString str = wxString::Format(stLosesTheMetropolis.c_str(), 
				swStringFormat.c_str(), name.c_str());
			RULE.Execute(shRuleUpdateNetworkUI, DataObject(str, playerIndex));

			RULE.Execute(shRuleAdjustPoints, DataObject(playerIndex, -2));

			// Remove their metropolis.
			PlayerGame::CornerObjectArray objects = 
				playerGame(playerIndex).getCornerObjects(shMetropolis);

			// Find the metropolis object we're looking for.  It's highly
			// likely that no one will ever get more than one, but it is
			// possible.
			CornerObjectPtr object;
			PlayerGame::CornerObjectArray::iterator it, itEnd = objects.end();
			for(it = objects.begin(); it != itEnd; ++it)
			{
				MetropolisObject* metro = 
					static_cast<MetropolisObject*>(it->get());
				if(metroType == metro->type())
				{
					object = (*it);
					break;
				}
			}
			wxASSERT(object);

			playerGame(playerIndex).removeCornerObject(shMetropolis, object);
		}

		// Update the UI.
		wxString str = wxString::Format(stTakesTheMetropolis.c_str(), 
			swStringFormat.c_str(), name.c_str());
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(str));

		// Add two points to the player's score.
		RULE.Execute(shRuleAdjustPoints, DataObject(owner, 2));

		Controller::get().Transmit(shEventPlayerUI, DataObject(GetGame(),
				owner));

		if(-1 != playerIndex)
		{
			Controller::get().Transmit(shEventPlayerUI, DataObject(GetGame(),
				playerIndex));
		}

		// It's possible that this changed the knight counts.
		Controller::get().Transmit(shEventKnightCounts, GetGame());

		// Back to the turn.
		gameData<HashString>(shState) = shTurn;
		RULE.Execute(shRuleRestartTurn, DataObject());
	}
};

IMPLEMENT_RULE(RuleSelectMetropolisCity, CITIES)
