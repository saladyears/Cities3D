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
// Class: LogicIsValidSettlementPlacementSeafarers
// 
// Determines if a settlement placement is valid in a Seafarers game.  A 
// placement is valid if the player has two ships connected from the placement.
//
// Derived From:
//     <Logic>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <Seafarers>
//
// Mixin To:
//     <LogicIsValidSettlementPlacement>
//
class LogicIsValidSettlementPlacementSeafarers : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		RULE.Decide(shLogicIsCornerSettleable, DataObject(
			input.read<CornerObject::TileCorner>(), shShips), output);

		// They are also not allowed to build settlements where all three tiles
		// are ocean.
		bool canBuild = output.read<bool>();
		if(true == canBuild)
		{
			const CornerObject::TileCorner &thisTile = 
				input.read<CornerObject::TileCorner>();
			wxInt32 tile1 = thisTile.first;
			wxInt32 corner = thisTile.second;
			wxInt32 tile2 = tile<IntArray>(shSides, tile1)[corner];
			wxInt32 tile3 = tile<IntArray>(shSides, tile1)[(corner + 5) % 6];

			// Test the corners for any land piece.
			DataObject input1(tile1), input2(tile2), input3(tile3);
			DataObject output1, output2, output3;
			RULE.Decide(shLogicTileIsLand, input1, output1);
			RULE.Decide(shLogicTileIsLand, input2, output2);
			RULE.Decide(shLogicTileIsLand, input3, output3);

			canBuild = output1.read<bool>() ||
					   output2.read<bool>() ||
					   output3.read<bool>();
		}

		output = DataObject(canBuild);
	}
};

IMPLEMENT_LOGIC_MIXIN(LogicIsValidSettlementPlacementSeafarers, 
					  LogicIsValidSettlementPlacement, SEAFARERS);
