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
#include "DummySelectionObject.h"
#include "CornerObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRestartTurnSaboteur
// 
// Restarts the game from a player sabotaging a city.
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
class RuleRestartTurnSaboteur : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
        // The current player is now in a blocking action.
        RULE.Execute(shRuleBeginBlockingAction, DataObject(current()));

		RULE.Execute(shRuleUpdateMessageUI, DataObject(stSabotageACity, 
            stWaitingSabotageACity));

		Controller::get().Transmit(shEventControlsUI, DataObject(false));
		Controller::get().Transmit(shEventBuildUI, 
				DataObject(false, GetGame()));
	
		// Transmit the selections.
		Controller::get().Transmit(shEventSelectionObject, 
			SelectionObjectPtr(
			new DummySelectionObject(shRuleSelectSaboteurCity)));

		// Make a list of all cities that can be sabotaged.
		PlayerGame::CornerObjectArray cities;
		FindCities(cities);

		// Now make them all selectable.
		RULE.Execute(shRuleResetTileFlags, DataObject(IGameObject::Dim));
		RULE.Execute(shRuleResetObjectFlags, DataObject(IGameObject::Dim));

		PlayerGame::CornerObjectArray::const_iterator it, 
			itEnd = cities.end();
		for(it = cities.begin(); it != itEnd; ++it)
		{
			CornerObjectPtr object = (*it);

			object->reset();
			object->set(IGameObject::Select);

			wxInt32 tile1 = object->tile1().first;
			wxInt32 tile2 = object->tile2().first;
			wxInt32 tile3 = object->tile3().first;

			tile(tile1)->reset();

			if(-1 != tile2)
			{
				tile(tile2)->reset();
			}

			if(-1 != tile3)
			{
				tile(tile3)->reset();
			}
		}
	}

private:
	void FindCities(PlayerGame::CornerObjectArray& cities)
	{
		std::set<CornerObject::TileCorner> blocked;

		wxInt32 curPlayer = current();
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			if(i != curPlayer)
			{
				// First, add metropolises to the set.
				const PlayerGame::CornerObjectArray& metropolis =
					playerGame(i).getCornerObjects(shMetropolis);
				{
					PlayerGame::CornerObjectArray::const_iterator it,
						itEnd = metropolis.end();
					for(it = metropolis.begin(); it != itEnd; ++it)
					{
						blocked.insert((*it)->tile1());
						blocked.insert((*it)->tile2());
						blocked.insert((*it)->tile3());
					}
				}

				// Now get the cities.
				const PlayerGame::CornerObjectArray& objects =
					playerGame(i).getCornerObjects(shCities);
				{
					PlayerGame::CornerObjectArray::const_iterator it,
						itEnd = objects.end();
					for(it = objects.begin(); it != itEnd; ++it)
					{
						const CornerObjectPtr& city = (*it);
						if(blocked.end() == blocked.find(city->tile1()))
						{
							cities.push_back(city);
						}
					}
				}
			}
		}
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RuleRestartTurnSaboteur, TurnSaboteur, 
						   RuleRestartGame, CITIES)
