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
#include "HexObject.h"
#include "TileObject.h"
#include "ChipObject.h"
#include "CornerObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleShowResources
// 
// Sets tile flags to show which tiles were rolled on.
//
// Derived From:
//     <Rule>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// Rules Called:
//     <RuleResetTileFlags>
//
// Selection Rule:
//     <>
//
// Game State:
//
//
// Transmits To:
//
//
class RuleShowResources : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		wxInt32 roll = object.read<wxInt32>();

		// Reset all tiles and objects to dim.
		RULE.Execute(shRuleResetTileFlags, DataObject(IGameObject::Dim));
		RULE.Execute(shRuleResetObjectFlags, DataObject(IGameObject::Dim));

		// Get the current robber tile to see if it needs to be noted.
		const HexObjectPtr &robber = GetGame()->getHexObject(shRobber);
		wxASSERT(robber);
		wxInt32 robberTile = robber->tile();

		// If the robber is hidden, he does not affect resources.
		if(true == robber->isset(IGameObject::Hidden))
		{
			robberTile = -1;
		}

		for(wxInt32 i = 0; i < numTiles(); ++i)
		{
			const TileObjectPtr &thisTile = tile(i);

			// Find any tile with this roll and set it as highlighted.
			const ChipObjectPtr &chip = thisTile->chip();
			if(chip)
			{
				if(roll == chip->roll())
				{
					thisTile->reset();

					// If it's also the robber, outline it in red.
					if(robberTile == i)
					{
						thisTile->set(TileObject::OutlineRed);
					}
					else
					{
						thisTile->set(TileObject::Outline);
					}

					// Check all player objects as well.
					for(wxInt32 j = 0; j < numPlayers(); ++j)
					{
						PlayerGame &game = playerGame(j);

						// Settlements.
						const PlayerGame::CornerObjectArray &settlements =
							game.getCornerObjects(shSettlements);

						for(size_t k = 0; k < settlements.size(); ++k)
						{
							HighlightObject(settlements[k], i);
						}

						// Cities.
						const PlayerGame::CornerObjectArray &cities =
							game.getCornerObjects(shCities);

						for(size_t k = 0; k < cities.size(); ++k)
						{
							HighlightObject(cities[k], i);
						}
					}
				}
			}
		}
	}

private:
	void HighlightObject(const CornerObjectPtr &object, const wxInt32 tileNum)
	{
		if( (object->tile1().first == tileNum) ||
			(object->tile2().first == tileNum) ||
			(object->tile3().first == tileNum))
		{
			object->reset();
			object->set(IGameObject::Outline);
		}
	}
};

IMPLEMENT_RULE(RuleShowResources, STANDARD)
