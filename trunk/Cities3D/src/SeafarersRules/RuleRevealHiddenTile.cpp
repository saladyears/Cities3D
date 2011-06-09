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
#include "Utility.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRevealHiddenTile
// 
// Reveals hidden tiles, if uncovered by a ship or road 
// placement, and adds a new resource to a user if the tile provides it.
//
// Derived From:
//     <Rule>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <HiddenTiles>
//
// Mixin To:
//     <RulePlaceShipCommon>
//
// Logic Called:
//     <LogicTileIsResource>
//
// Transmits To:
//     <eventGameObjects>
//
class RuleRevealHiddenTile : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxInt32 thisTile, side;
		boost::tie(thisTile, side) = Utility::decodeSel(
			object.read<wxInt32>());

		//look at both possible tiles to see if they are hidden tiles
		wxInt32 tile1 = tile<IntArray>(
			shSides, thisTile)[((side + 1) % 6)];
		wxInt32 tile2 = tile<IntArray>(
			shSides, thisTile)[((side + 5) % 6)];

		wxInt32 hiddenTile = -1;

		if( (-1 != tile1) && 
			(shHidden == tile<HashString>(shTileType, tile1)))
		{
			hiddenTile = tile1;
		}

		if( (-1 != tile2) && 
			(shHidden == tile<HashString>(shTileType, tile2)))
		{
			hiddenTile = tile2;
		}

		if(-1 != hiddenTile)
		{
			RULE.Execute(shRuleRewardHiddenTile, DataObject(hiddenTile));
		}
	}
};

IMPLEMENT_RULE_MIXIN(RuleRevealHiddenTile, RulePlaceShipCommon, HIDDEN_TILES)
IMPLEMENT_RULE_MIXIN(RuleRevealHiddenTile, RulePlaceRoad, HIDDEN_TILES)
