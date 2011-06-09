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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePrepareUICities
// 
// Relinks all extra objects with their corner objects.
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
//     <RulePrepareUI>
//
class RulePrepareUICities : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		wxInt32 players = numPlayers();

		// Go through each player's cities and relink city walls to them.
		for(wxInt32 i = 0; i < players; ++i)
		{
			PlayerGame::CornerObjectArray walls = 
				playerGame(i).getCornerObjects(shWalls);

			PlayerGame::CornerObjectArray cities = 
				playerGame(i).getCornerObjects(shCities);
			std::for_each(cities.begin(), cities.end(),
				boost::bind(&RulePrepareUICities::Relink, this,
				_1, walls));

			// Now, hook up metropolises.
			PlayerGame::CornerObjectArray metros =
				playerGame(i).getCornerObjects(shMetropolis);
			std::for_each(cities.begin(), cities.end(),
				boost::bind(&RulePrepareUICities::RelinkMetros, this,
				_1, metros));
		}
	}

private:
	void Relink(CornerObjectPtr& city, PlayerGame::CornerObjectArray& walls)
	{
		CornerObject::TileCorner tileCorner = city->tile1();

		PlayerGame::CornerObjectArray::iterator it, itEnd = walls.end();
		for(it = walls.begin(); it != itEnd; ++it)
		{
			CornerObjectPtr wall = (*it);

			// Find walls on the same tile.
			if(true == wall->onTileCorner(tileCorner))
			{
				// Find the lowest thing below the city.
				CornerObjectPtr object = city;
				CornerObjectPtr below = object->below().lock();
				
				while(below)
				{
					object = below;
					below = object->below().lock();
				}

				wall->above(object);
				object->below(wall);
			}
		}
	}

	void RelinkMetros(CornerObjectPtr& city, 
		PlayerGame::CornerObjectArray& metros)
	{
		CornerObject::TileCorner tileCorner = city->tile1();

		PlayerGame::CornerObjectArray::iterator it, itEnd = metros.end();
		for(it = metros.begin(); it != itEnd; ++it)
		{
			CornerObjectPtr metro = (*it);

			// Find metros on the same tile.
			if(true == metro->onTileCorner(tileCorner))
			{
				city->above(metro);
				metro->below(city);
			}
		}
	}
};

IMPLEMENT_RULE_MIXIN(RulePrepareUICities, RulePrepareUI, CITIES)
