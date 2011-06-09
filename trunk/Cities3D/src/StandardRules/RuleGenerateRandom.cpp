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
#include "Map.h"
#include "GameEngine.h"
#include "RNG.h"
#include "UtilityMap.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleGenerateRandom
//
// Generates random TileType values for <TileObjects>
// marked as random.  Uses the RandomTiles and RandomChips
// data fields in the currently loaded <Map> as the random
// data.
//
// Derived From:
//     <Rule>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <RandomTiles>
//
// Mixin To:
//     <RuleGenerateBoard>
//
// Logic Called:
//     <LogicTileIsOcean>,
//     <LogicTileIsResource>
//
class RuleGenerateRandom : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		//this function fills in values marked with MAP_RANDOM in the board 
		//with random tile values that are defined in the map data; random 
		//values are always visible from the beginning of the game, they are 
		//just randomly assigned
		HashStringArray values;
		
		//get random tiles ready
		wxInt32 size;
		size = UtilityMap::prepareDataArray(MAP.read<Data::IntHash>(
			shRandomResources), values);

		//sanity
		wxASSERT(size == MAP.read<wxInt32>(shRandomTiles));

		//now set the values on the map
		wxInt32 tileNum = 0;
		for(wxInt32 i = 0; i < numTiles(); ++i)
		{
			HashString &type = tile<HashString>(shTileType, i);
			
			if(shRandom == type)
			{
				type = values[tileNum++];

				DataObject input(i), output;
				RULE.Decide(shLogicTileIsOcean, input, output);
				
				//sea values cannot be initial placements or secondary.
				if(true == output.read<bool>())
				{
					tile<wxInt32>(shInitial, i) = FALSE;
					tile<wxInt32>(shSecondary, i) = FALSE;
				}
			}
		}
		wxASSERT(tileNum == MAP.read<wxInt32>(shRandomTiles));

		//see if it's even necessary to look for random chips
		wxInt32 chips = MAP.read<wxInt32>(shRandomChips);
		if(0 == chips)
		{
			return;
		}

		//get random chits ready
		size = UtilityMap::prepareDataArray(MAP.read<Data::IntHash>(
			shRandomChipArray), values);
		wxASSERT(size == chips);
		
		//now set the roll on the map
		wxInt32 chipNum = 0;
		for(wxInt32 i = 0; i < numTiles(); i++)
		{
			DataObject input(i), output;
			RULE.Decide(shLogicTileIsResource, input, output);
			
			wxInt32 &roll = tile<wxInt32>(shRoll, i);

			//set the values if we can
			if( (true == output.read<bool>()) &&
				(-1 == roll))
			{
				roll = boost::lexical_cast<wxInt32>(values[chipNum]);
				++chipNum;
			}
		}
		wxASSERT(chipNum == chips);

		bool bValid;
		do
		{
			//validate the values to ensure no 6's and 8's are near each other
			bValid = ValidateChips();
		}
		while(false == bValid);
	}

private:
	//ensure that no 6's and 8's are next to each other on the board for 
	//random chips
	bool ValidateChips()
	{
		//NOTE: This algorithm assumes that any map that uses this algorithm 
		//has *ALL* tiles started as random tiles.  This is a valid assumption 
		//for the A New World official maps. Other official maps have random 
		//tiles without all tiles being marked random, such as Standard and New
		//Shores, but their layout algorithm is handled in other RuleSets.

		IntArray chipTiles;
		chipTiles.reserve(numTiles());

		//first compile the list of other tiles with chips on them
		for(wxInt32 i = 0; i < numTiles(); ++i)
		{
			DataObject input(i), output;
			RULE.Decide(shLogicTileIsResource, input, output);

			if(true == output.read<bool>())
			{
				chipTiles.push_back(i);
			}
		}

		size_t size = chipTiles.size();
		IntArray index(size, 0);
		for(size_t i = 0; i < size; ++i)
		{
			index[i] = i;
		}

		//now randomize all the indices
		for(size_t i = 0; i < size; ++i)
		{
			std::swap(index[i], index[RAND.pooled(size)]);
		}

		bool valid = true;
		
		//now we traverse the list, stopping at each valid chit tile to see if
		//it is a 6 or an 8 and if it is, then reassigning it
		for(wxInt32 i = 0; i < numTiles(); ++i)
		{
			wxInt32 roll = tile<wxInt32>(shRoll, i);
			
			//check for 6 or 8
			if( (6 != roll) && 
				(8 != roll))
			{
				continue;
			}

			//now we check each side to ensure that it is not surrounded by 
			//another 6 or 8
			for(wxInt32 j = 0; j < 6; ++j)
			{
				wxInt32 side = tile<IntArray>(shSides, i)[j];
				wxInt32 otherRoll = tile<wxInt32>(shRoll, side);
				
				//if it's a six or an eight, we've got a problem
				if( (6 == otherRoll) || 
					(8 == otherRoll))
				{
					valid = false;
					break;
				}
			}

			//if valid, continue
			if(true == valid)
			{
				continue;
			}

			//traverse the chit index looking for a swap
			for(size_t j = 0; j < size; j++)
			{
				//get the switch value
				wxInt32 switchVal = chipTiles[index[j]];
				wxInt32 switchRoll = tile<wxInt32>(shRoll, switchVal);

				//if it's the same as the tile we're trying to switch, continue
				if(switchVal == i)
				{
					continue;
				}

				//if the switch tile is also a 6 or 8 continue, since that 
				//would accomplish nothing
				if( (6 == switchRoll) || 
					(8 == switchRoll))
				{
					continue;
				}

				//now check to see if it is valid
				bool switchFlag = true;
				for(wxInt32 k = 0; k < 6; ++k)
				{
					wxInt32 side = tile<IntArray>(shSides, switchVal)[k];
					wxInt32 otherRoll = tile<wxInt32>(shRoll, side);

					//if it's a six or an eight, we can't use this tile
					if( (6 == otherRoll) || 
						(8 == otherRoll))
					{
						switchFlag = false;
						break;
					}
				}

				if(false == switchFlag)
				{
					continue;
				}

				//since we've made it here, we can switch the values

				//get the roll values
				std::swap(tile<wxInt32>(shRoll, switchVal),
						  tile<wxInt32>(shRoll, i));
				
				//since we had to switch, continue again
				return false;
			}
		}
		
		return true;
	}
};

IMPLEMENT_RULE_MIXIN(RuleGenerateRandom, RulePreGenerateBoard, RANDOM_TILES);

