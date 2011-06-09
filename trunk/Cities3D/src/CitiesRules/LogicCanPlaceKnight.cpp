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
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicCanPlaceKnight
// 
// Determines the available board locations for a knight to be placed for 
// the current player.
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
// Logic Called:
//     <LogicIsValidKnightPlacement>
//
class LogicCanPlaceKnight : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		mPlayer = input.read<wxInt32>();

		// Algorithm steps:
		//
		// 1. Create a set of all possible tile and corner locations, ensuring
		//    that there are no repeats.
		// 2. Iterate through each potential location.
		// 3. For each potential location, run logic that determines if a 
		//    knight can be built there.  If not, remove the location from
		//    the set.
		//

		TileCornerSet corners;

		// Step 1: Build the valid set of knight locations.
		BuildValidSet(corners);
		
		// Now that we've got a set of all possible locations, iterate through
		// it and remove ones that are invalid.
		TileCornerSet::iterator it = corners.begin(), itEnd = corners.end();
		TileCornerSet::iterator itThis;

		while(it != itEnd)
		{
			itThis = it++;	

			DataObject input(*itThis, mPlayer), output;
			RULE.Decide(shLogicIsValidKnightPlacement, input, output);

			if(false == output.read<bool>())
			{
				corners.erase(itThis);
			}
		}

		// Return the final placement selection.
		output = DataObject(corners);
	}

private:
	typedef std::pair<wxInt32, wxInt32> TileCorner;
	typedef std::set<TileCorner> TileCornerSet;

	void BuildInvalidSet(TileCornerSet &invalids)
	{
		PlayerGame::CornerObjectArray objects;

		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			const PlayerGame &thisGame = playerGame(i);
			thisGame.getAllCornerObjects(objects);
		}

		// Build the hash.
		PlayerGame::CornerObjectArray::const_iterator it, 
			itEnd = objects.end();

		for(it = objects.begin(); it != itEnd; ++it)
		{
			invalids.insert((*it)->tile1());
			invalids.insert((*it)->tile2());
			invalids.insert((*it)->tile3());
		}
	}

	void BuildValidSet(TileCornerSet &corners)
	{
		TileCornerSet invalids;

		BuildInvalidSet(invalids);
		
		wxInt32 tiles = numTiles();
		
		// Iterate over all of the tiles, ignoring border tiles and invalid 
		// sides, adding tile/corner combinations.  To ensure no repeats, use
		// the lowest numbered corner of any non-border tiles out of the three.
		for(wxInt32 i = 0; i < tiles; ++i)
		{
			// Skip border tiles.
			if(FALSE == tile<wxInt32>(shBorder, i))
			{
				for(wxInt32 j = 0; j < 6; ++j)
				{
					TileCorner side(i, j);

					// It must be a valid side.
					if(invalids.end() == invalids.find(side))
					{
						bool insert = true;

						// Take sides 2 or 3 if side 0 or 1 is a border.  Take
						// sides 4 or 5 only if sides 2 or 3 AND sides 0 or 1
						// are borders.
						if(2 <= j)
						{
							if(4 <= j)
							{
								wxInt32 index = (j + 5) % 6;
								wxInt32 side = tile<IntArray>(shSides, i)[index];
								wxASSERT(-1 != side);

								if(FALSE == tile<wxInt32>(shBorder, side))
								{
									insert = false;
								}
							}

							wxInt32 side = tile<IntArray>(shSides, i)[j];
							wxASSERT(-1 != side);
							if(FALSE == tile<wxInt32>(shBorder, side))
							{
								insert = false;
							}
						}

						if(true == insert)
						{
							corners.insert(side);
						}
					}
				}
			}
		}
	}

	wxInt32 mPlayer;
};

IMPLEMENT_LOGIC(LogicCanPlaceKnight, CITIES)
