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
#include "CornerObject.h"
#include "Utility.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleReduceCityVolcanoCities
// 
// Handles moving or removing a metropolis object if a city is reduced by the
// volcano.
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
// Mixin To:
//     <RuleReduceCity>
//
class RuleReduceCityVolcanoCities : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(2 <= object.numItems());

		wxInt32 tile, corner;
		boost::tie(tile, corner) = Utility::decodeSel(object.read<wxInt32>());
		wxInt32 player = object.read<wxInt32>(1);

		CornerObject::TileCorner tileCorner(tile, corner);

		// Look for a metropolis object on this corner for the player.  If one
		// exists, it either gets moved to another city if the player has one,
		// or it is taken off the board until the player builds a city.
		const PlayerGame::CornerObjectArray& metropolis =
			playerGame(player).getCornerObjects(shMetropolis);

		CornerObjectPtr obj;

		PlayerGame::CornerObjectArray::const_iterator it, 
			itEnd = metropolis.end();
		for(it = metropolis.begin(); it != itEnd; ++it)
		{
			if(true == (*it)->onTileCorner(tileCorner))
			{
				obj = (*it);
				break;
			}
		}

		// Found one.
		if(obj)
		{
			// See if the player has another city to put it on.
			const PlayerGame::CornerObjectArray& cities =
				playerGame(player).getCornerObjects(shCities);

            // They just lost this city which had a metropolis on it.  So, if 
            // they have the same amount of remaining cities as they have 
            // metropolises, by definition, one will be open to receive the
            // metropolis.
			if(cities.size() >= metropolis.size())
			{
				wxASSERT(true == playerGameData<IntArray>(shVolcanoMetropolis, 
					player).empty());

				// They need to select an alternate city.
				playerGameData<wxInt32>(shMoveMetropolis, player) = TRUE;
				gameData<wxInt32>(shOriginalPlayer) = current();
				gameData<HashString>(shState) = shVolcanoMetropolis;
				gameData<HashString>(shNextVolcanoRule) =
					shRuleRestartVolcanoMetropolis;
			}
			
			// The metropolis object disappears, until it is moved to its new
			// home.
			obj->reset();
			obj->set(IGameObject::Hidden);

			// Keep track of the id for both possibilities.
			playerGameData<IntArray>(shVolcanoMetropolis, 
				player).push_back(obj->id());
		}
	}
};

IMPLEMENT_RULE_MIXIN(RuleReduceCityVolcanoCities, RuleReduceCityCommon, 
					 VOLCANO_CITIES)
