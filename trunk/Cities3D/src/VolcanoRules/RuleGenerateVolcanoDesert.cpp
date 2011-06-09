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
#include "RNG.h"
#include "HexObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleGenerateVolcanoDesert
// 
// Replaces deserts with volcanoes.  A volcano may not be placed on as an
// initial settlement.
//
// Derived From:
//     <Rule>
//
// Project:
//     <VolcanoRules>
//
// RuleSet:
//     <VolcanoDesert>
//
// Mixin To:
//     <RulePostGenerateBoard>
//
class RuleGenerateVolcanoDesert : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		wxInt32 tiles = numTiles();
		
		const HexObjectPtr& robber = GetGame()->getHexObject(shRobber);
		wxASSERT(robber);

		wxInt32 robberTile = robber->tile();

		for(wxInt32 i = 0; i < tiles; ++i)
		{
			// Look for desert tiles.
			if(shDesert == tile<HashString>(shTileType, i))
			{
				tile<HashString>(shTileType, i) = shVolcano;
				tile<wxInt32>(shInitial, i) = FALSE;

				// The volcano also gets a random chip, but it may not be a
				// 6 or an 8.
				wxInt32 num = RAND.pooled(8);

				// 0-3 maps to 2-5, 4-7 maps to 9-12
				num = (num < 4) ? num + 2 : num + 5;

				tile<wxInt32>(shRoll, i) = num;

				// If the robber is on this tile, hide it.
				if(robberTile == i)
				{
					robber->set(IGameObject::Hidden);
				}
			}
		}
	}
};

IMPLEMENT_RULE_MIXIN(RuleGenerateVolcanoDesert, RulePostGenerateBoard, 
					 VOLCANO_DESERT);
