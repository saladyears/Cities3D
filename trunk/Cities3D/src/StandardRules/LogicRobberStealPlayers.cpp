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
#include "RuleSetDatabase.h"
#include "CornerObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicRobberStealPlayers
// 
// Gathers all the players that can be stolen from on a given tile.
//
// Derived From:
//     <Logic>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
class LogicRobberStealPlayers : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		wxASSERT(1 <= input.numItems());

		wxInt32 thisTile = input.read<wxInt32>();
		wxInt32 curPlayer = current();

		typedef std::set<wxInt32> IntSet;
		IntSet players;
		PlayerGame::CornerObjectArray objects;
		IntArray owners;
		
		// Go through each player, looking for settlements and cities on the
		// tile.  If any exist, that player may be stolen from.
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			if(i != curPlayer)
			{
				// See if the player can actually be stolen from.
				wxInt32 total = 0;
				Data::IntHash& resources = 
					playerGameData<Data::IntHash>(shResources, i);
				Data::IntHash::iterator it, itEnd = resources.end();
				for(it = resources.begin(); it != itEnd; ++it)
				{
					total += it->second;
				}

				// Only add them if they have a card to steal.
				if(0 < total)
				{
					const PlayerGame::CornerObjectArray& settlements = 
						playerGame(i).getCornerObjects(shSettlements);
					PlayerGame::CornerObjectArray::const_iterator it, 
						itEnd = settlements.end();
					for(it = settlements.begin(); it != itEnd; ++it)
					{
						CornerObject::TileCorner tile1 = (*it)->tile1();
						CornerObject::TileCorner tile2 = (*it)->tile2();
						CornerObject::TileCorner tile3 = (*it)->tile3();

						if( (thisTile == tile1.first) ||
							(thisTile == tile2.first) ||
							(thisTile == tile3.first))
						{
							players.insert(i);
							objects.push_back(*it);
							owners.push_back(i);
						}
					}

					const PlayerGame::CornerObjectArray& cities = 
						playerGame(i).getCornerObjects(shCities);
					itEnd = cities.end();
					for(it = cities.begin(); it != itEnd; ++it)
					{
						CornerObject::TileCorner tile1 = (*it)->tile1();
						CornerObject::TileCorner tile2 = (*it)->tile2();
						CornerObject::TileCorner tile3 = (*it)->tile3();

						if( (thisTile == tile1.first) ||
							(thisTile == tile2.first) ||
							(thisTile == tile3.first))
						{
							players.insert(i);
							objects.push_back(*it);
							owners.push_back(i);
						}
					}
				}
			}
		}

		output = DataObject(players, objects, owners);
	}
};

IMPLEMENT_LOGIC(LogicRobberStealPlayers, STANDARD)
