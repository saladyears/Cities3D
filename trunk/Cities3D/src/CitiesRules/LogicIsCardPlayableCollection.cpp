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
#include "Logic.h"
#include "LogicHashMixin.h"
#include "RuleSetDatabase.h"
#include "CornerObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicIsCardPlayableCollection
// 
// Determines whether the given collection card is playable right now.
//
// Derived From:
//     <Logic>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
class LogicIsCardPlayableCollection : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		const HashString& type = input.read<HashString>();

		// The player must have a settlement or city on the given type of tile.
		PlayerGame::CornerObjectArray objects = 
			playerGame().getCornerObjects(shSettlements);
		const PlayerGame::CornerObjectArray& cities =
			playerGame().getCornerObjects(shCities);
		std::copy(cities.begin(), cities.end(), std::back_inserter(objects));

		typedef std::set<wxInt32> IntSet;
		IntSet tiles;

		PlayerGame::CornerObjectArray::const_iterator it,
			itEnd = objects.end();
		for(it = objects.begin(); it != itEnd; ++it)
		{
			const CornerObjectPtr& object = (*it);

			// Check all three tiles.
			CornerObject::TileCorner tile1 = object->tile1();
			CornerObject::TileCorner tile2 = object->tile2();
			CornerObject::TileCorner tile3 = object->tile3();

			if(type == tile<HashString>(shTileType, tile1.first))
			{
				tiles.insert(tile1.first);
			}

			if(type == tile<HashString>(shTileType, tile2.first))
			{
				tiles.insert(tile2.first);
			}

			if(type == tile<HashString>(shTileType, tile3.first))
			{
				tiles.insert(tile3.first);
			}
		}

		output = DataObject(tiles);
	}
};

IMPLEMENT_LOGIC(LogicIsCardPlayableCollection, CITIES)
