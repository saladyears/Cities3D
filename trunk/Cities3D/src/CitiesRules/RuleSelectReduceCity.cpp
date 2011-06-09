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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleSelectReduceCity
// 
// Handles a player selecting a city to destroy.
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
//
class RuleSelectReduceCity : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		// In network games, some players will not have their city reduced,
		// and others will.  Because each player needs to see their city
		// reduce options as if they are the current player, on their local
		// machine, they are set to the current player.  However, when they
		// select a city to reduce, this rule gets sent to everyone because
		// it is a top-level rule.  Therefore, everyone else will receive this
		// rule and try and execute it on the wrong player, in the case where
		// the player reducing a city is not the actual current player.  To
		// correct this, we first check to see if the current player actually
		// has a city on this corner.  If they don't, we will search every
		// other player's cities until we find the city, then set the current
		// player to that player for the execution of this rule.
		wxInt32 id = object.read<wxInt32>();

		wxLogDebug(wxT("Reduced city id: %d"), id);
		
		bool found = FindCity(current(), id);

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

				if(true == FindCity(i, id))
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

		// This player has no longer lost a city.
		playerGameData<wxInt32>(shLoseCity) = FALSE;

		// Reduce the city.
		RULE.Execute(shRuleReduceCity, object);

		// Reset the current player back to the turn player.
		wxInt32 original = gameData<wxInt32>(shOriginalPlayer);
		gameData<wxInt32>(shCurrentPlayer) = original;

		RULE.Execute(shRuleRestartReduceCities, DataObject());
	}

private:
	bool FindCity(wxInt32 player, wxInt32 id)
	{
		const PlayerGame::CornerObjectArray& cities = 
			playerGame(player).getCornerObjects(shCities);

		bool found = false;
		PlayerGame::CornerObjectArray::const_iterator it, itEnd = cities.end();
		for(it = cities.begin(); it != itEnd; ++it)
		{
			if((*it)->id() == id)
			{
				found = true;
				break;
			}
		}

		return found;
	}
};

IMPLEMENT_RULE(RuleSelectReduceCity, CITIES)
