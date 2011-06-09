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
#include "Utility.h"
#include "ExtraCornerObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePlaceCityVolcanoCities
// 
// Moves a metropolis to a newly placed city if it was on a volcano destroyed
// city.
//
// Derived From:
//     <Rule>
//     <IUndoable>
//
// Project:
//     <VolcanoRules>
//
// RuleSet:
//     <VolcanoCities>
//
// Mixin To:
//     <RulePlaceCity>
//
class RulePlaceCityVolcanoCities : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		// If we have metropolises to place, find the city.
		IntArray& volcanoMetropolis = 
			playerGameData<IntArray>(shVolcanoMetropolis);

		if(false == volcanoMetropolis.empty())
		{
			wxInt32 tile, corner;
			boost::tie(tile, corner) = Utility::decodeSel(object.read<wxInt32>());
			CornerObject::TileCorner tileCorner(tile, corner);

			// Find the city.
			CornerObjectPtr city;
			const PlayerGame::CornerObjectArray& cities =
				playerGame().getCornerObjects(shCities);
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
			wxASSERT(city);

			// Use the first metropolis, if there are more than 1.
			IntArray::iterator itV = volcanoMetropolis.begin();
			wxInt32 id = *itV;
			volcanoMetropolis.erase(itV);

			CornerObjectPtr metro;
			const PlayerGame::CornerObjectArray& metropolis = 
				playerGame().getCornerObjects(shMetropolis);
			itEnd = metropolis.end();
			for(it = metropolis.begin(); it != itEnd; ++it)
			{
				if((*it)->id() == id)
				{
					metro = (*it);
					break;
				}
			}
			wxASSERT(metro);

			// Move the metropolis to the new location.
			city->below(metro);
			metro->above(city);

			CornerObject::TileCorner tile1 = city->tile1();
			metro->tileCorner(tile1.first, tile1.second, GetGame());
			metro->reset();

			// Adjust coords.
			Vector coords = metro->coords();
			coords.y = city->coords().y;
			metro->coords(coords);
		}
	}
};

IMPLEMENT_RULE_MIXIN(RulePlaceCityVolcanoCities, RulePlaceCity, VOLCANO_CITIES)
