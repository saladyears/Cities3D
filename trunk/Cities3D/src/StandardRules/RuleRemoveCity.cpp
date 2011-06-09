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
#include "CityObject.h"
#include "AggregatorObject.h"
#include "Utility.h"
#include "Controller.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRemoveCity
// 
// Removes a city from the board at the given tile and corner for the
// current player.
//
// Derived From:
//     <Rule>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
class RuleRemoveCity : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		wxInt32 player = current();

		// See if a particular player is having it removed.
		if(2 <= object.numItems())
		{
			player = object.read<wxInt32>(1);
		}

		// Get the tile and corner being removed.
		wxInt32 tile, corner;
		boost::tie(tile, corner) = Utility::decodeSel(object.read<wxInt32>());

		// Find the city in the player's current list.
		const PlayerGame::CornerObjectArray &cities = 
			playerGame(player).getCornerObjects(shCities);

		CornerObject::TileCorner tileCorner(tile, corner);

		CornerObjectPtr city;
		PlayerGame::CornerObjectArray::const_iterator it, 
			itEnd = cities.end();
		for(it = cities.begin(); it != itEnd; ++it)
		{
			if(true == (*it)->onTileCorner(tileCorner))
			{
				city = (*it);
				break;
			}
		}

		// Remove it from the player's list.
		playerGame(player).removeCornerObject(shCities, city);

		// Add one to the player's stock cities.
		playerGameData<wxInt32>(shStockCities, player) += 1;

		// Subtract two points from the player's score.
		RULE.Execute(shRuleAdjustPoints, DataObject(player, -2));
	}
};

IMPLEMENT_RULE(RuleRemoveCity, STANDARD)
