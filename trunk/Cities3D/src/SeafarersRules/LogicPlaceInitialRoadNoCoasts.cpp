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
#include "GameEngine.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
typedef std::pair<wxInt32, wxInt32> TileCorner;

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicPlaceInitialRoadNoCoasts
// 
// Prevents an initial road from being built along a coast,
// unless the coast is a border tile.
//
// Derived From:
//     <Logic>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <InitialShips>
//
// Mixin To:
//     <LogicPlaceInitialRoad>
//
// Logic Called:
//     <LogicTileIsOcean>
//
class LogicPlaceInitialRoadNoCoasts : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		bool valid = true;

		if(2 <= input.numItems())
		{
			wxInt32 tile1, corner1;
			boost::tie(tile1, corner1) = input.read<TileCorner>(1);

			wxInt32 tile2 = tile<IntArray>(shSides, tile1)[corner1];
			wxInt32 tile3 = tile<IntArray>(shSides, tile1)[(corner1 + 5) % 6];

			wxInt32 oceanCount = 0;
			wxInt32 borderCount = 0;

			// They are allowed to build a road only in the following cases:
			//
			// 1.  All three tiles are land.
			// 2.  One tile is an ocean and is a border tile.
			// 3.  Two tiles are oceans and are border tiles.

			RULE.Decide(shLogicTileIsOcean, DataObject(tile1), output);
			if(true == output.read<bool>())
			{
				++oceanCount;
				if(TRUE == tile<wxInt32>(shBorder, tile1))
				{
					++borderCount;
				}
			}
			output.reset();

			RULE.Decide(shLogicTileIsOcean, DataObject(tile2), output);
			if(true == output.read<bool>())
			{
				++oceanCount;
				if(TRUE == tile<wxInt32>(shBorder, tile2))
				{
					++borderCount;
				}
			}
			output.reset();

			RULE.Decide(shLogicTileIsOcean, DataObject(tile3), output);
			if(true == output.read<bool>())
			{
				++oceanCount;
				if(TRUE == tile<wxInt32>(shBorder, tile3))
				{
					++borderCount;
				}
			}

			valid = false;

			if(oceanCount == borderCount)
			{
				valid = true;
			}
		}
			
		output = DataObject(valid);
	}
};

IMPLEMENT_LOGIC_MIXIN(LogicPlaceInitialRoadNoCoasts, LogicPlaceInitialRoad, 
					  INITIAL_SHIPS);
