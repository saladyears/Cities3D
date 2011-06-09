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
#include "ExtraCornerObject.h"
#include "UtilityGL.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleCornerDestroyedSingleBonus
// 
// Handles removing extra points when a corner is destroyed by a volcano.
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
class RuleCornerDestroyedSingleBonus : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		// So, when this happens, the player is losing a settlement or city 
		// in a map that gives an extra tile for each new secondary area the
		// player colonizes.  If the player has another city or settlement on
		// the island, the players extra point should be transferred to that
		// city or settlement.  If there is no other city or settlement, the
		// extra point should be removed.

		// Hooray for shared_ptrs.
		const CornerObjectPtr& obj = object.read<CornerObjectPtr>();
		wxInt32 owner = obj->owner();

		// Examine all extra objects the player owns to see if any fall on
		// this tile/corner.
		CornerObjectPtr extra;

		CornerObject::TileCorner tileCorner = obj->tile1();
		const PlayerGame::CornerObjectArray& extras = 
			playerGame(owner).getCornerObjects(shExtra);
		PlayerGame::CornerObjectArray::const_iterator it, itEnd = extras.end();
		for(it = extras.begin(); it != itEnd; ++it)
		{
			if((*it)->onTileCorner(tileCorner))
			{
				extra = (*it);
				break;
			}
		}

		// If we found an extra object, spring to action!
		if(extra)
		{
			// Find another settlement or city on the same land mass.
			CornerObjectPtr alternate = FindAlternate(tileCorner.first, owner);

			// If we found one, move the extra point to it.
			if(alternate)
			{
				alternate->below(extra);
				extra->above(alternate);

				ExtraCornerObject* ext = 
					static_cast<ExtraCornerObject*>(extra.get());
				ext->offset(ext->offset());

				//determine coordinates
				tileCorner = alternate->tile1();
				extra->tileCorner(tileCorner.first, tileCorner.second,
					GetGame());
			}
			// Otherwise, remove the extra.
			else
			{
				playerGame(owner).removeCornerObject(shExtra, extra);

				// They also lose 1 point.
				RULE.Execute(shRuleAdjustPoints, DataObject(owner, -1));

				// Notify the UI.
				RULE.Execute(shRuleUpdateNetworkUI, 
                    DataObject(stLosesBonusPoint, owner));
			}
		}
	}

private:
	typedef std::set<wxInt32> TileSet;

	CornerObjectPtr FindAlternate(wxInt32 tile, wxInt32 player)
	{
		CornerObjectPtr object;

		TileSet tileSet;
		Iterate(tile, tileSet);

		// Now determine if the player has any cities or settlements to
		// transfer the object to.
		PlayerGame::CornerObjectArray objects =
			playerGame(player).getCornerObjects(shSettlements);
		const PlayerGame::CornerObjectArray& cities =
			playerGame(player).getCornerObjects(shCities);

		std::copy(cities.begin(), cities.end(), std::back_inserter(objects));

		PlayerGame::CornerObjectArray::const_iterator it, 
			itEnd = objects.end();
		for(it = objects.begin(); it != itEnd; ++it)
		{
			const CornerObjectPtr& thisObject = (*it);
			
			wxInt32 tile1 = thisObject->tile1().first;
			wxInt32 tile2 = thisObject->tile2().first;
			wxInt32 tile3 = thisObject->tile3().first;

			// See if any are in the list.
			if( (tileSet.end() != tileSet.find(tile1)) ||
				(tileSet.end() != tileSet.find(tile2)) ||
				(tileSet.end() != tileSet.find(tile3)))
			{
				object = thisObject;
				break;
			}
		}
		
		return object;
	}

	void Iterate(wxInt32 thisTile, TileSet& tileSet)
	{
		tileSet.insert(thisTile);

		// Find all secondary tiles attached to this one.
		for(wxInt32 i = 0; i < 6; ++i)
		{
			wxInt32 otherTile = tile<IntArray>(shSides, thisTile)[i];

			if( (-1 != otherTile) &&
				(tileSet.end() == tileSet.find(otherTile)) &&
				(TRUE == tile<wxInt32>(shSecondary, otherTile)))
			{
				Iterate(otherTile, tileSet);
			}
		}
	}
};

IMPLEMENT_RULE_MIXIN(RuleCornerDestroyedSingleBonus, 
					 RuleCornerDestroyedVolcano, VOLCANO_SINGLE_BONUS)
