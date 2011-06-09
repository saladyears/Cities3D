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
// Class: RuleReduceCityCities
// 
// Handles city walls cleanup if a city is reduced.
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
//     <RuleReduceCity>
//
class RuleReduceCityCities : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(3 <= object.numItems());

		wxInt32 tile, corner;
		boost::tie(tile, corner) = Utility::decodeSel(object.read<wxInt32>());
		wxInt32 player = object.read<wxInt32>(1);
		wxInt32 id = object.read<wxInt32>(2);

		CornerObject::TileCorner tileCorner(tile, corner);

		// See if any city walls exist on this corner for the player.
		PlayerGame::CornerObjectArray walls = 
			playerGame(player).getCornerObjects(shWalls);

		CornerObjectPtr wall;

		PlayerGame::CornerObjectArray::const_iterator it, itEnd = walls.end();
		for(it = walls.begin(); it != itEnd; ++it)
		{
			if(true == (*it)->onTileCorner(tileCorner))
			{
				wall = (*it);
				break;
			}
		}

		// If we have one, remove it.
		if(wall)
		{
			playerGame(player).removeCornerObject(shWalls, wall);

			// Add one to the player's stock walls.
			playerGameData<wxInt32>(shStockWalls, player) += 1;

			// They are now allowed 2 less cards in hand.
			playerGameData<wxInt32>(shAllowedCards, player) -= 2;

			// Update the UI.
			RULE.Execute(shRuleUpdateNetworkUI, DataObject(stLosesACityWalls, 
                player));
		}

		// Also see if the city had been sabotaged before it was reduced.
		{
			IntArray& sabotaged = playerGameData<IntArray>(shSabotagedCities,
				player);
			IntArray::iterator it = std::find(sabotaged.begin(), 
				sabotaged.end(), id);
			if(sabotaged.end() != it)
			{
				sabotaged.erase(it);
			}
		}
	}
};

IMPLEMENT_RULE_MIXIN(RuleReduceCityCities, RuleReduceCityCommon, CITIES)
