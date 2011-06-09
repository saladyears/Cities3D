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
#include "SideObject.h"
#include "CornerObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicIsValidSidePlacement
// 
// Determines if a side placement is valid in a standard game.  A side 
// placement is valid if the player has another of the given type of side
// in any of the four branch positions from the placement, or if they have
// a valid corner object on either end.
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
class LogicIsValidSidePlacement : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		const HashString type = input.read<HashString>();
		const wxInt32 index = input.read<wxInt32>(1);
		const wxInt32 side = input.read<wxInt32>(2);

		wxInt32 curTile = index;
		wxInt32 curSide = 0;

		bool canPlace = false;

		// Determine the four branch possibilities.  There are actually eight
		// possibilities we must test because it could be one tile/side or the
		// opposite tile/side for each branch possibility.

		typedef std::pair<wxInt32, wxInt32> TileSide;
		typedef std::vector<TileSide> TileSideVector;

		TileSideVector sides;
		sides.reserve(8);

		// Roads may not be built past blocking cities or settlements.
		CornerSet blocked;
		BuildBlockedCorners(blocked);

		CornerObject::TileCorner corner(curTile, side);

		if(blocked.end() == blocked.find(corner))
		{
			// Branch 1.
			curSide = (side + 5) % 6;
			sides.push_back(TileSide(curTile, curSide));
			sides.push_back(TileSide(tile<IntArray>(shSides, curTile)[curSide],
				(curSide + 3) % 6));

			curTile = tile<IntArray>(shSides, curTile)[side];

			// Branch 2.
			curSide = (side + 4) % 6;
			sides.push_back(TileSide(curTile, curSide));
			sides.push_back(TileSide(tile<IntArray>(shSides, curTile)[curSide],
				(curSide + 3) % 6));
		}

		curTile = index;
		corner = CornerObject::TileCorner(curTile, (side + 1) % 6);

		if(blocked.end() == blocked.find(corner))
		{
			// Branch 3.
			curSide = (side + 1) % 6;
			sides.push_back(TileSide(curTile, curSide));
			sides.push_back(TileSide(tile<IntArray>(shSides, curTile)[curSide],
				(curSide + 3) % 6));
			
			curTile = tile<IntArray>(shSides, curTile)[side];

			// Branch 4.
			curSide = (side + 2) % 6;
			sides.push_back(TileSide(curTile, curSide));
			sides.push_back(TileSide(tile<IntArray>(shSides, curTile)[curSide],
				(curSide + 3) % 6));
		}

		// Now that we have all the possible branches, search the player's
		// objects to see if they have any at any of the branch locations.
		const PlayerGame::SideObjectArray &objects = 
			playerGame().getSideObjects(type);

		// All we have to do is find out if any of the branches have a road on
		// them.
		for(size_t i = 0; i < sides.size(); ++i)
		{
			for(size_t j = 0; j < objects.size(); ++j)
			{
				if( (sides[i] == objects[j]->tile1()) ||
					(sides[i] == objects[j]->tile2()))
				{
					canPlace = true;
					break;
				}
			}
		}

		// Check for settlements/cities to start from.
		if(false == canPlace)
		{
			CornerObject::TileCorner corner1(index, side);
			CornerObject::TileCorner corner2(index, (side + 1) % 6);

			const PlayerGame::CornerObjectArray& settlements =
				playerGame().getCornerObjects(shSettlements);

			PlayerGame::CornerObjectArray::const_iterator it,
				itEnd = settlements.end();
			for(it = settlements.begin(); it != itEnd; ++it)
			{
				if( (true == (*it)->onTileCorner(corner1)) ||
					(true == (*it)->onTileCorner(corner2)))
				{
					canPlace = true;
					break;
				}
			}

			if(false == canPlace)
			{
				const PlayerGame::CornerObjectArray& cities =
					playerGame().getCornerObjects(shCities);

				PlayerGame::CornerObjectArray::const_iterator it,
					itEnd = cities.end();
				for(it = cities.begin(); it != itEnd; ++it)
				{
					if( (true == (*it)->onTileCorner(corner1)) ||
						(true == (*it)->onTileCorner(corner2)))
					{
						canPlace = true;
						break;
					}
				}
			}
		}

		output = DataObject(canPlace);
	}

private:
	typedef std::set<CornerObject::TileCorner> CornerSet;

	void BuildBlockedCorners(CornerSet& blocked)
	{
		// Sides are blocked by all corner objects.
		
		wxInt32 curPlayer = current();

		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			if(i == curPlayer)
			{
				continue;
			}

			PlayerGame::CornerObjectArray objects;
			playerGame(i).getAllCornerObjects(objects);

			PlayerGame::CornerObjectArray::const_iterator it, 
				itEnd = objects.end();

			for(it = objects.begin(); it != itEnd; ++it)
			{
				blocked.insert((*it)->tile1());
				blocked.insert((*it)->tile2());
				blocked.insert((*it)->tile3());
			}
		}
	}
};

IMPLEMENT_LOGIC(LogicIsValidSidePlacement, STANDARD);
