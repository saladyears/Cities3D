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
#include "CornerObject.h"
#include "Utility.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleSelectVolcanoMetropolis
// 
// Handles a player selecting a city to destroy.
//
// Derived From:
//     <Rule>
//
// Project:
//     <VolcanoRules>
//
// RuleSet:
//     <VolcanoCities>
//
//
class RuleSelectVolcanoMetropolis : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		// This follows the same execution path as RuleSelectVolcanoMetropolis, since
		// it is technically possible for two players to have separate
		// metropolises moved by a volcanic eruption at the same time. 
		// Completely ridiculous, but still possible, so we have to account for
		// it.
		wxInt32 id = object.read<wxInt32>();
		
		CornerObject::TileCorner tileCorner;
		bool found = FindCity(current(), id, tileCorner);

		// We didn't find the city in the current player.  Find the right 
		// player.
		if(false == found)
		{
			wxInt32 thisPlayer = -1;
			wxInt32 curPlayer = current();
			for(wxInt32 i = 0; i < numPlayers(); ++i)
			{
				if(i == curPlayer)
				{
					continue;
				}

				if(true == FindCity(i, id, tileCorner))
				{
					thisPlayer = i;
					break;
				}
			}

			wxASSERT(-1 != thisPlayer);

            // Set them.
			gameData<wxInt32>(shCurrentPlayer) = thisPlayer;
		}

        // This player has ended a blocking action.
        RULE.Execute(shRuleEndBlockingAction, DataObject(current()));


		// This player no longer moves their metropolis
		playerGameData<wxInt32>(shMoveMetropolis) = FALSE;

		// Move the metropolis.
		RULE.Execute(shRulePlaceCityVolcanoCities, DataObject(
			Utility::encodeSel(tileCorner.first, tileCorner.second)));
		
		// Reset the current player back to the turn player.
		wxInt32 original = gameData<wxInt32>(shOriginalPlayer);
		gameData<wxInt32>(shCurrentPlayer) = original;

		RULE.Execute(shRuleRestartVolcanoMetropolis, DataObject());
	}

private:
	bool FindCity(wxInt32 player, wxInt32 id, 
		CornerObject::TileCorner& tileCorner)
	{
		const PlayerGame::CornerObjectArray& cities = 
			playerGame(player).getCornerObjects(shCities);

		bool found = false;
		PlayerGame::CornerObjectArray::const_iterator it, itEnd = cities.end();
		for(it = cities.begin(); it != itEnd; ++it)
		{
			if((*it)->id() == id)
			{
				tileCorner = (*it)->tile1();
				found = true;
				break;
			}
		}

		return found;
	}
};

IMPLEMENT_RULE(RuleSelectVolcanoMetropolis, VOLCANO_CITIES)
