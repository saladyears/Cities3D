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
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicCanPlaceRoad
// 
// Determines the available board locations for a road to be placed for the
// current player.
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
// Logic Called:
//     <LogicIsValidRoadPlacement>
//
class LogicCanPlaceRoad : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		// Algorithm steps:
		//
		// 1. Create a set of all possible tile and side locations, ensuring
		//    that there are no repeats.
		// 2. Iterate through each potential location.
		// 3. For each potential location, run logic that determines if a road
		//    can be built there.  If not, remove the location from the set.
		//

		TileSideSet sides;

		// Step 1: Build the valid set of road locations.
		BuildValidSet(sides);
		
		// Now that we've got a set of all possible locations, iterate through
		// it and remove ones that are invalid.
		TileSideSet::iterator it = sides.begin(), itEnd = sides.end();
		TileSideSet::iterator itThis;

		while(it != itEnd)
		{
			itThis = it++;	

			DataObject input(itThis->first, itThis->second), output;
			RULE.Decide(shLogicIsValidRoadPlacement, input, output);

			if(false == output.read<bool>())
			{
				sides.erase(itThis);
			}
		}

		// Return the final placement selection.
		output = DataObject(sides);
	}

private:
	typedef std::pair<wxInt32, wxInt32> TileSide;
	typedef std::set<TileSide> TileSideSet;

	void BuildInvalidSet(TileSideSet &invalids)
	{
		PlayerGame::SideObjectArray objects;

		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			const PlayerGame &thisGame = playerGame(i);
			thisGame.getAllSideObjects(objects);
		}

		// Build the hash.
		PlayerGame::SideObjectArray::const_iterator it, itEnd = objects.end();

		for(it = objects.begin(); it != itEnd; ++it)
		{
			invalids.insert((*it)->tile1());
			invalids.insert((*it)->tile2());
		}
	}

	void BuildValidSet(TileSideSet &sides)
	{
		TileSideSet invalids;

		BuildInvalidSet(invalids);
		
		wxInt32 tiles = numTiles();
		
		// Iterate over all of the tiles, ignoring border tiles and invalid 
		// sides, adding tile/side combinations.  To ensure no repeats, only 
		// use the first three sides (0-3), unless the opposite tile is a 
		// border.
		for(wxInt32 i = 0; i < tiles; ++i)
		{
			// Skip border tiles.
			if(FALSE == tile<wxInt32>(shBorder, i))
			{
				for(wxInt32 j = 0; j < 6; ++j)
				{
					TileSide side(i, j);

					// It must be a valid side.
					if(invalids.end() == invalids.find(side))
					{
						bool insert = true;

						// Only take the highest numbered sides if they touch a
						// border tile.

						wxInt32 otherTile = tile<IntArray>(shSides, i)[j];
						wxASSERT(-1 != otherTile);

						if( (3 <= j) &&
							(FALSE == tile<wxInt32>(shBorder, otherTile)))
						{
							insert = false;
						}

						// Also skip sides where both tiles are ocean.
						DataObject ocean1(i), output1;
						RULE.Decide(shLogicTileIsOcean, ocean1, output1);

						DataObject ocean2(otherTile), output2;
						RULE.Decide(shLogicTileIsOcean, ocean2, output2);

						if( (true == output1.read<bool>()) &&
							(true == output2.read<bool>()))
						{
							insert = false;
						}

						if(true == insert)
						{
							sides.insert(side);
						}
					}
				}
			}
		}
	}
};

IMPLEMENT_LOGIC(LogicCanPlaceRoad, STANDARD)
