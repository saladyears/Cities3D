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
// Class: RuleRevealChiplessTile
// 
// Reveals chipless tiles, if uncovered by a ship or road placement.
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
//     <RulePlaceShipCommon>
//     <RulePlaceRoad> 
//
class RuleRevealChiplessTile : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxInt32 thisTile, side;
		boost::tie(thisTile, side) = Utility::decodeSel(
			object.read<wxInt32>());

		//look at both possible tiles to see if they are chipless tiles
		wxInt32 tile1 = tile<IntArray>(
			shSides, thisTile)[((side + 1) % 6)];
		wxInt32 tile2 = tile<IntArray>(
			shSides, thisTile)[((side + 5) % 6)];

		wxInt32 chiplessTile = -1;

		if( (-1 != tile1) && 
			(TRUE == tile<wxInt32>(shChipless, tile1)))
		{
			chiplessTile = tile1;
		}

		if( (-1 != tile2) && 
			(TRUE == tile<wxInt32>(shChipless, tile2)))
		{
			chiplessTile = tile2;
		}

		if(-1 != chiplessTile)
		{
			RULE.Execute(shRuleRewardChiplessTile, DataObject(chiplessTile));
		}
	}
};

IMPLEMENT_RULE_MIXIN(RuleRevealChiplessTile, RulePlaceShipCommon, 
					 CHIPLESS_TILES)
IMPLEMENT_RULE_MIXIN(RuleRevealChiplessTile, RulePlaceRoad, CHIPLESS_TILES)
