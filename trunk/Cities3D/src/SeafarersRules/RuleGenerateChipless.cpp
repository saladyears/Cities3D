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
#include "UtilityMap.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleGenerateChipless
// 
// Generates chipless tiles. Chipless tiles do not start off with 
// a roll chip on them, but as the game progresses, get roll chips 
// added to them.  At first they are drawn from a fixed pool of chips,
// but once that is gone, they are swapped from the starting section
// of the board, so that players are forced to expand to new areas 
// or else lose their roll chips to those that have already expanded.
//
// Derived From:
//     <Rule>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <ChiplessTiles>
//
// Mixin To:
//     <RuleGenerateBoard>
//
class RuleGenerateChipless : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		HashStringArray values;

		//prepare chitless tiles
		wxInt32 size;
		size = UtilityMap::prepareDataArray(MAP.read<Data::IntHash>(
			shChiplessResources), values);

		//sanity
		wxInt32 tiles = MAP.read<wxInt32>(shChiplessTiles);
		wxASSERT(size == tiles);

		(void) tiles;	//remove compiler warning in Release mode

		//now set the tiles on the map
		for(wxInt32 i = 0, j = 0; i < numTiles(); i++)
		{
			if(shChipless ==  tile<HashString>(shTileType, i))
			{
				tile<HashString>(shTileType, i) = values[j++];
				tile<wxInt32>(shChipless, i) = TRUE;
			}
		}

		//prepare chitless chits (which are stored in the game data for later 
		//revealing)
		size = UtilityMap::prepareDataArray(MAP.read<Data::IntHash>(
			shChiplessChipArray), gameData<HashStringArray>(
			shChiplessChipArray));
		wxASSERT(size == MAP.read<wxInt32>(shChiplessChips));;
	}
};

IMPLEMENT_RULE_MIXIN(RuleGenerateChipless, RuleGenerateBoard, CHIPLESS_TILES);
