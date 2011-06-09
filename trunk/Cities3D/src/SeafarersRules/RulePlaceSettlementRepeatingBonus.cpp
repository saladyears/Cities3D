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
#include "IUndoable.h"
#include "RuleSetDatabase.h"
#include "Utility.h"
#include "ExtraObject.h"
#include "Controller.h"
#include "AggregatorObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePlaceSettlementRepeatingBonus
// 
// Handles awarding of bonus extra points for secondary islands.
//
// Derived From:
//     <Rule>
//     <IUndoable>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <RepeatingBonus>
//
// Mixin To:
//     <RulePlaceSettlement>
//
class RulePlaceSettlementRepeatingBonus 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxInt32 secondaryTile = -1;
		wxInt32 secondaryCorner = -1;

		wxInt32 tile1, corner1;
		boost::tie(tile1, corner1) = 
			Utility::decodeSel(object.read<wxInt32>());

		if(TRUE == tile<wxInt32>(shSecondary, tile1))
		{
			secondaryTile = tile1;
			secondaryCorner = corner1;
		}

		// Determine the other two tile possibilities.
		wxInt32 tile2 = tile<IntArray>(shSides, tile1)[corner1];
		if( (-1 != tile2) &&
			(TRUE == tile<wxInt32>(shSecondary, tile2)))
		{
			secondaryTile = tile2;
			secondaryCorner = (corner1 + 4) % 6;
		}

		wxInt32 tile3 = tile<IntArray>(shSides, tile1)[(corner1 + 5) % 6];
		if( (-1 != tile3) &&
			(TRUE == tile<wxInt32>(shSecondary, tile3)))
		{
			secondaryTile = tile3;
			secondaryCorner = (corner1 + 2) % 6;
		}

		CornerObjectPtr settlement;

		playerGameData<wxInt32>(shReceivedBonus) = FALSE;

		// They get a new bonus every time they get to a new secondary tile.
		if(	(-1 != secondaryTile) &&
			(true == DetermineBonus(secondaryTile, secondaryCorner, 
			settlement)))
		{
			playerGameData<wxInt32>(shReceivedBonus) = TRUE;

			wxASSERT(settlement);

			wxInt32& bonus = playerGameData<wxInt32>(shBonus);

			// Keep track of how many bonii they've gotten.
			bonus++;

			// However, they are only awarded a maximum of 2 points.
			wxInt32 points = std::min(bonus, 2);

			wxInt32 turn = gameData<wxInt32>(shTurn);

			// Create the extra object and adjust the settlement.
			CornerObjectPtr above = settlement;
			AggregatorObjectPtr aggregate(new AggregatorObject);

			for(wxInt32 i = 0; i < points; ++i)
			{
				CornerObjectPtr extra(new ExtraObject(current(), turn, CR_SIZE,
					GetGame(), tile1, corner1, above));
				above->below(extra);
				playerGame().addCornerObject(shExtra, extra);
				aggregate->add(extra);
				above = extra;
			}

			Controller::get().Transmit(shEventGameObjects, aggregate);

			// Add one point to the player's score.
			RULE.Execute(shRuleAdjustPoints, DataObject(current(), points));

			// Update the UI.'
			wxString str;

			if(1 == points)
			{
				str = stReceivesOneForColonizing;
			}
			else
			{
				str = wxString::Format(stReceivesXForColonizing.c_str(), 
                    swStringFormat.c_str(), points);
			}

			RULE.Execute(shRuleUpdateNetworkUI, DataObject(str));

			// Play the sound.
			RULE.Execute(shRulePlaySound, DataObject(SOUND_EXTRA_POINTS));
		}
	}

	virtual void Unexecute(const DataObject &object)
	{
		if(TRUE == playerGameData<wxInt32>(shReceivedBonus))
		{
			playerGameData<wxInt32>(shReceivedBonus) = FALSE;

			wxInt32 tile1, corner1;
			boost::tie(tile1, corner1) = 
				Utility::decodeSel(object.read<wxInt32>());

			wxInt32& bonus = playerGameData<wxInt32>(shBonus);
			wxInt32 points = std::min(bonus, 2);

			--bonus;

			wxInt32 turn = gameData<wxInt32>(shTurn);

			// Remove extra objects.
			for(wxInt32 i = 0; i < points; ++i)
			{
				CornerObjectPtr extra(new ExtraObject(current(), turn, CR_SIZE,
					GetGame(), tile1, corner1, boost::weak_ptr<CornerObject>()));
				playerGame().removeCornerObject(shExtra, extra);
			}

			// Subtract one point from the player's score.
			RULE.Execute(shRuleAdjustPoints, DataObject(current(), -points));
		}
	}

	virtual bool CanUndo() const
	{
		// This should never stop an undo chain.
		return true;
	}

private:
	typedef std::set<wxInt32> TileSet;

	bool DetermineBonus(wxInt32 thisTile, wxInt32 thisCorner, 
		CornerObjectPtr& object)
	{
		bool bonus = true;

		TileSet tileSet;

		// Create a set of all secondary tiles attached to this tile.
		Iterate(thisTile, tileSet);

		// Now go through all the current settlements and cities for the 
		// player and see if any already exist on any of these tiles.
		PlayerGame::CornerObjectArray objects =
			playerGame().getCornerObjects(shSettlements);
		const PlayerGame::CornerObjectArray& cities =
			playerGame().getCornerObjects(shCities);

		std::copy(cities.begin(), cities.end(), std::back_inserter(objects));

		PlayerGame::CornerObjectArray::const_iterator it, 
			itEnd = objects.end();
		for(it = objects.begin(); it != itEnd; ++it)
		{
			const CornerObjectPtr& thisObject = (*it);
			
			// Ignore the settlement they just put down.
			if(false == thisObject->onTileCorner(std::make_pair(
				thisTile, thisCorner)))
			{
				wxInt32 tile1 = thisObject->tile1().first;
				wxInt32 tile2 = thisObject->tile2().first;
				wxInt32 tile3 = thisObject->tile3().first;

				// See if any are in the list.
				if( (tileSet.end() != tileSet.find(tile1)) ||
					(tileSet.end() != tileSet.find(tile2)) ||
					(tileSet.end() != tileSet.find(tile3)))
				{
					bonus = false;
					break;
				}
			}
			else
			{
				object = thisObject;
			}
		}
		
		return bonus;
	}

	void Iterate(wxInt32 thisTile, TileSet& tileSet)
	{
		tileSet.insert(thisTile);

		// Go through each of the sides, looking at each tile to see if it is
		// a land tile.  If it is, and it's not already in the set, iterate
		// on it.
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

IMPLEMENT_RULE_MIXIN(RulePlaceSettlementRepeatingBonus, 
					 RulePlaceSettlementTurn, REPEATING_BONUS)
