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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleGenerateAroundBoardPorts
// 
// Generates ports around the outside of a map, alternating
// port-ocean or ocean-port until all random ports are placed.
//
// Derived From:
//     <Rule>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <AroundTheBoardPorts>
//
// Replaces:
//     <RuleGeneratePorts>
//
class RuleGenerateAroundBoardPorts : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		//this function takes the random port allocations in the map and 
		//assigns them to the places on the board that are marked as random 
		//ports
		HashStringArray values;

		//get the array ready
		wxInt32 size;
		size = UtilityMap::prepareDataArray(MAP.read<Data::IntHash>(
			shRandomPorts), values);
		wxASSERT(size == MAP.read<wxInt32>(shPorts));

		//the ports and sea tiles need to alternate around the edge of the 
		//board
		wxInt32 ocean;

		//set the random ocean number
		ocean = RAND.pooled(2);

		wxInt32 numLand = MAP.read<wxInt32>(shLandTiles);

		//run through the sea array
		for(wxInt32 i = 0; i < size; i++)
		{
			//set the non ocean types
			tile<HashString>(shTileType, numLand + (i << 1) + ocean) = 
				values[i];

			//set the ocean types
			tile<HashString>(shTileType, numLand + (i << 1) + !ocean) = 
				shOcean;
		}
	}
};

IMPLEMENT_RULE_REPLACE(RuleGenerateAroundBoardPorts, RuleGeneratePorts, 
					   AROUND_THE_BOARD_PORTS);
