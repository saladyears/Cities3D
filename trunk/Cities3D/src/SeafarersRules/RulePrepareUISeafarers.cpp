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
// Class: RulePrepareUISeafarers
// 
// Relinks all extra objects with their corner objects.
//
// Derived From:
//     <Rule>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <Seafarers>
//
// Mixin To:
//     <RulePrepareUI>
//
class RulePrepareUISeafarers : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		wxInt32 players = numPlayers();

		// Go through each player's cities and settlements, and relink extra
		// objects to them.
		for(wxInt32 i = 0; i < players; ++i)
		{
			PlayerGame::CornerObjectArray extras = 
				playerGame(i).getCornerObjects(shExtra);

			PlayerGame::CornerObjectArray settlements = 
				playerGame(i).getCornerObjects(shSettlements);
			std::for_each(settlements.begin(), settlements.end(),
				boost::bind(&RulePrepareUISeafarers::Relink, this,
				_1, extras));

			PlayerGame::CornerObjectArray cities = 
				playerGame(i).getCornerObjects(shCities);
			std::for_each(cities.begin(), cities.end(),
				boost::bind(&RulePrepareUISeafarers::Relink, this,
				_1, extras));
		}
	}

private:
	void Relink(CornerObjectPtr& object, PlayerGame::CornerObjectArray& extras)
	{
		CornerObject::TileCorner tileCorner = object->tile1();
		CornerObjectPtr above = object;

		PlayerGame::CornerObjectArray::iterator it, itEnd = extras.end();
		for(it = extras.begin(); it != itEnd; ++it)
		{
			CornerObjectPtr extra = (*it);

			// Find extras on the same tile.
			if(true == extra->onTileCorner(tileCorner))
			{
				extra->above(above);
				above->below(extra);
				above = extra;
			}
		}
	}
};

IMPLEMENT_RULE_MIXIN(RulePrepareUISeafarers, RulePrepareUI, SEAFARERS)
