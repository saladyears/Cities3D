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
#include "IRuleEngine.h"
#include "Controller.h"
#include "DummySelectionObject.h"
#include "CornerObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRestartMetropolis
// 
// Restarts the game from a player placing a metropolis.
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
class RuleRestartMetropolis : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
        // The current player is now in a blocking action.
        RULE.Execute(shRuleBeginBlockingAction, DataObject(current()));

		// Shut everything down.
		Controller::get().Transmit(shEventBuildUI, 
			DataObject(false, GetGame()));
		Controller::get().Transmit(shEventControlsUI, 
			DataObject(false));

		DummySelectionObject* selection = new DummySelectionObject(
			shRuleSelectMetropolisCity);

		Controller::get().Transmit(shEventSelectionObject, 
			SelectionObjectPtr(selection));

		RULE.Execute(shRuleResetObjectFlags, DataObject(IGameObject::Dim));
		RULE.Execute(shRuleResetTileFlags, DataObject(IGameObject::Dim));

		PlayerGame::CornerObjectArray cities = 
			playerGame().getCornerObjects(shCities);
		wxASSERT(false == cities.empty());

		PlayerGame::CornerObjectArray::iterator it, itEnd = cities.end();
		for(it = cities.begin(); it != itEnd; ++it)
		{
			CornerObjectPtr object = (*it);

			// Any city with a metropolis on it cannot have another metropolis
			// added..
			if(!object->above().lock())
			{
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

		RULE.Execute(shRuleUpdateMessageUI, DataObject(stPlaceYourMetropolis, 
            stWaitingPlaceAMetropolis));
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RuleRestartMetropolis, Metropolis, RuleRestartGame, 
						   CITIES)
