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
#include "CornerObject.h"
#include "Controller.h"
#include "define/defineGL.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleSelectRepairCity
// 
// Repairs the selected city.
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
//     <RulePlayCard>
//
class RuleSelectRepairCity : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxInt32 id = object.read<wxInt32>();
		
		// Find the city object this id belongs to.
		CornerObjectPtr city;

		const PlayerGame::CornerObjectArray& cities = 
			playerGame().getCornerObjects(shCities);

		PlayerGame::CornerObjectArray::const_iterator it, 
			itEnd = cities.end();
		for(it = cities.begin(); it != itEnd; ++it)
		{
			if((*it)->id() == id)
			{
				city = (*it);
				break;
			}
		}
		wxASSERT(city);

		// Rever the city to normal.
		city->rotation(Vector(0.0, 0.0, 0.0));
		Vector coords = city->coords();
		coords.y -= (0.5 * SCALE);
		coords.x -= (0.5 * SCALE);
		city->coords(coords);

		// Remove this city to the player's list of sabotaged cities.
		{
			IntArray& sabotaged = playerGameData<IntArray>(shSabotagedCities);
			IntArray::iterator it = std::find(sabotaged.begin(), 
				sabotaged.end(), id);
			wxASSERT(sabotaged.end() != it);
			sabotaged.erase(it);
		}

		// Update the UI.
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stRepairsACity));

		// Play the sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_REPAIR_CITY));

		gameData<HashString>(shState) = shTurn;

		// Run an animation if needed.
		RULE.Execute(shRuleAnimatePlacement, DataObject(playerGame().player(),
			GameObjectPtr(city), playerGame().color()));

		// Adjust the resources used.
		DataObject input(0), output;
		RULE.Decide(shLogicCostRepairCity, input, output);
		RULE.Execute(shRulePurchase, output);

		// Adjust stats.
		RULE.Execute(shRulePurchaseItem, DataObject(current(), shCityRepair));

		Controller::get().Transmit(shEventDirty, 0);

		RULE.Execute(shRuleRestartTurn, DataObject());
	}
};

IMPLEMENT_RULE(RuleSelectRepairCity, CITIES)
