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
// Class: RuleSelectSaboteurCity
// 
// Sabotages the selected city.
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
class RuleSelectSaboteurCity : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
        // The current player has ended a blocking action.
        RULE.Execute(shRuleEndBlockingAction, DataObject(current()));

		wxInt32 id = object.read<wxInt32>();
		
		// We don't know which player the city belongs to, so look through
		// all cities on the board.
		wxInt32 cityPlayer = -1;
		wxInt32 curPlayer = current();
		CornerObjectPtr city;

		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			if(i != curPlayer)
			{
				const PlayerGame::CornerObjectArray& cities = 
					playerGame(i).getCornerObjects(shCities);

				PlayerGame::CornerObjectArray::const_iterator it, 
					itEnd = cities.end();
				for(it = cities.begin(); it != itEnd; ++it)
				{
					if((*it)->id() == id)
					{
						cityPlayer = i;
						city = (*it);
						break;
					}
				}
			}
		}
		wxASSERT(city);

		// Set the city on its side.
		city->rotation(Vector(0.0, 0.0, 90.0));
		Vector coords = city->coords();
		coords.y += (0.5 * SCALE);
		coords.x += (0.5 * SCALE);
		city->coords(coords);

		// Add this city to the player's list of sabotaged cities.
		playerGameData<IntArray>(shSabotagedCities, cityPlayer).push_back(id);

		// Play the sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_SELECT_SABOTEUR_CITY));

		wxString str = wxString::Format(stSabotagesACityOf.c_str(),
			swStringFormat.c_str(), playerGame(cityPlayer).player().Name().c_str());
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(str));

		Controller::get().Transmit(shEventDirty, 0);

		RULE.Execute(shRulePlayProgressCardEnd, DataObject());
	}
};

IMPLEMENT_RULE(RuleSelectSaboteurCity, CITIES)
