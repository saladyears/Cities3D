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
#include "IRuleEngine.h"
#include "Controller.h"
#include "AggregatorObject.h"
#include "ChipObject.h"
#include "RNG.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRewardHiddenTileVolcano
// 
// Hidden tiles may be deserts, in which case the discovered tile should be
// a Volcano.
//
// Derived From:
//     <Rule>
//
// Project:
//     <VolcanoRules>
//
// RuleSet:
//     <VolcanoSingleBonus>
//
// Mixin To:
//     <RulePlaceCityTUrn>
//
class RuleRewardHiddenTileVolcano : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxInt32 hiddenTile = object.read<wxInt32>();

		// If it's a desert, terraform it.
		if(shDesert == tile<HashString>(shTileType, hiddenTile))
		{
			tile<HashString>(shTileType, hiddenTile) = shVolcano;

			// The volcano also gets a random chip, but it may not be a
			// 6 or an 8.
			wxInt32 num = RAND.pooled(8);

			// 0-3 maps to 2-5, 4-7 maps to 9-12
			num = (num < 4) ? num + 2 : num + 5;

			tile<wxInt32>(shRoll, hiddenTile) = num;

			//create a new chip object
			AggregatorObjectPtr aggregate(new AggregatorObject);
			tile(hiddenTile)->createChipObject(true);
			tile(hiddenTile)->chip()->randomizeRotation();
			aggregate->add(tile(hiddenTile)->chip());

			//notify the display engine
			Controller::get().Transmit(shEventGameObjects, aggregate);
		}
	}
};

IMPLEMENT_RULE_MIXIN(RuleRewardHiddenTileVolcano, RuleRewardHiddenTile,
					 VOLCANO_HIDDEN_TILES)
