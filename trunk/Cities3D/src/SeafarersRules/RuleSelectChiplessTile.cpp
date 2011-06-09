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
#include "Controller.h"
#include "CornerObject.h"
#include "DummySelectionObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleSelectChiplessTile
// 
// Determines which tiles are legal for the player to select from when moving
// a chip from the mainland to an island.
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
class RuleSelectChiplessTile : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// The current player is now in a blocking action.
        RULE.Execute(shRuleBeginBlockingAction, DataObject(current()));

		BuildHashes();

		// First, determine the set of tiles that can be selected by this
		// player.
		IntSet tileSet;
		DetermineTiles(tileSet);

		//set the dummy selection
		DummySelectionObject *pSelection = new DummySelectionObject(
			shRuleExecuteChiplessTile);

		//send the dummy selection object up
		Controller::get().Transmit(shEventSelectionObject, 
			SelectionObjectPtr(pSelection));

		// Make sure eveything is hidden.
		Controller::get().Transmit(shEventBuildUI, 
			DataObject(false, GetGame()));
		Controller::get().Transmit(shEventControlsUI, DataObject(false));

		// Set all these tiles as selectable.
		RULE.Execute(shRuleResetObjectFlags, DataObject(IGameObject::Dim));
		RULE.Execute(shRuleResetTileFlags, DataObject(IGameObject::Dim));

		IntSet::const_iterator it, itEnd = tileSet.end();
		for(it = tileSet.begin(); it != itEnd; ++it)
		{
			wxInt32 tileID = (*it);
			tile(tileID)->reset();
			tile(tileID)->set(IGameObject::Select);
		}

		wxString str = stSelectAChipFromTheMainland;
		wxString str2 = stWaitingSelectAChipFromTheMainland;
		RULE.Execute(shRuleUpdateMessageUI, DataObject(str, str2));

		mPlayerTiles.clear();
	}

private:
	typedef std::set<wxInt32> IntSet;

	void BuildHashes()
	{
		// Go through and record the current player's tiles.
		wxInt32 curPlayer = current();
		
		const PlayerGame::CornerObjectArray& settlements = 
			playerGame(curPlayer).getCornerObjects(shSettlements);

		const PlayerGame::CornerObjectArray& cities =
			playerGame(curPlayer).getCornerObjects(shCities);

		PlayerGame::CornerObjectArray::const_iterator it,
			itEnd = settlements.end();

		for(it = settlements.begin(); it != itEnd; ++it)
		{
			const CornerObjectPtr &object = (*it);

			mPlayerTiles.insert(object->tile1().first);
			mPlayerTiles.insert(object->tile2().first);
			mPlayerTiles.insert(object->tile3().first);
		}

		itEnd = cities.end();
		for(it = cities.begin(); it != itEnd; ++it)
		{
			const CornerObjectPtr &object = (*it);

			mPlayerTiles.insert(object->tile1().first);
			mPlayerTiles.insert(object->tile2().first);
			mPlayerTiles.insert(object->tile3().first);
		}
	}

	void DetermineTiles(IntSet& tileSet)
	{
		// The rules of this scenario are as follows:
		//
		// 1.  Start with all tiles that still have a chip on them on the
		//     mainland.
		// 2.  The red tokens (6 & 8) may not be placed next to one another
		//     on the small island.  If the tile being placed on the island
		//     has a 6 or 8 tile touching it, remove all 6 and 8s from the
		//     mainland set.  If there are no tiles left after this, revert
		//     to the previous set and use it.
		// 3.  All settlements and cities on the mainland must be adjacent to
		//     at least one number.  Remove all tiles from the set if removing
		//     the tile would leave a city or settlement with no adjacent
		//     numbers.  If there are no tiles left after this, revert to the
		//     previous set and use it.
		// 4.  The player must remove tokens from tiles adjacent to their
		//     cities or settlements.  Remove all tiles from the set if they
		//     are not adjacent to a player's city or settlement.  If there
		//     are no tiles left after this, revert to the previous set and
		//     use it.

		// First see if all rules can be supported.
		Rule0(tileSet);
		Rule3(tileSet);
		Rule2(tileSet);
		Rule1(tileSet);

		// If there's nothing left to choose, get rid of the third rule and
		// try again.
		if(true == tileSet.empty())
		{
			Rule0(tileSet);
			Rule2(tileSet);
			Rule1(tileSet);

			// Again, check for a valid set.
			if(true == tileSet.empty())
			{
				Rule0(tileSet);
				Rule1(tileSet);

				// And finally, the last choice.
				if(true == tileSet.empty())
				{
					Rule0(tileSet);
				}
			}
		}
	}

	void Rule0(IntSet& tileSet)
	{
		wxASSERT(true == tileSet.empty());

		// Start, by recording all tiles that have chips on them that are
		// marked with the Initial flag.
		wxInt32 count = numTiles();
		for(wxInt32 i = 0; i < count; ++i)
		{
			if( (TRUE == tile<wxInt32>(shInitial, i)) &&
				(-1 != tile<wxInt32>(shRoll, i)))
			{
				tileSet.insert(i);
			}
		}
	}

	void Rule1(IntSet& tileSet)
	{
		wxInt32 chiplessTile = gameData<wxInt32>(shChiplessTile);
		
		bool red = false;

		// See if this tile has any neighboring tiles with a roll of 6 or 8.
		for(wxInt32 i = 0; i < 6; ++i)
		{
			wxInt32 side = tile<IntArray>(shSides, chiplessTile)[i];
			wxInt32 roll = tile<wxInt32>(shRoll, side);

			if( (6 == roll) ||
				(8 == roll))
			{
				red = true;
				break;
			}
		}

		// If we have a red chip, remove all red chips from the set.
		if(true == red)
		{
			IntSet::iterator it = tileSet.begin(), itThis, 
				itEnd = tileSet.end();
			
			while(it != itEnd)
			{
				itThis = it++;

				wxInt32 roll = tile<wxInt32>(shRoll, (*itThis));
				if( (6 == roll) ||
					(8 == roll))
				{
					tileSet.erase(itThis);
				}
			}
		}
	}

	void Rule2(IntSet& tileSet)
	{
		// Go through every player's settlements and cities, and subtract
		// tiles from the set where it is the only tile that has a chip
		// touching a settlement or city.
		wxInt32 players = numPlayers();
		for(wxInt32 i = 0; i < players; ++i)
		{
			const PlayerGame::CornerObjectArray& settlements = 
			playerGame(i).getCornerObjects(shSettlements);

			const PlayerGame::CornerObjectArray& cities =
				playerGame(i).getCornerObjects(shCities);

			PlayerGame::CornerObjectArray::const_iterator it,
				itEnd = settlements.end();

			for(it = settlements.begin(); it != itEnd; ++it)
			{
				const CornerObjectPtr &object = (*it);
				size_t rolls = 0;

				wxInt32 tile1 = object->tile1().first;
				wxInt32 tile2 = object->tile2().first;
				wxInt32 tile3 = object->tile3().first;

				// See if there's only 1 tile with a roll
				if(-1 != tile<wxInt32>(shRoll, tile1))
				{
					++rolls;
				}

				if(-1 != tile<wxInt32>(shRoll, tile2))
				{
					++rolls;
				}

				if(-1 != tile<wxInt32>(shRoll, tile3))
				{
					++rolls;
				}

				if(1 >= rolls)
				{
					tileSet.erase(tile1);
					tileSet.erase(tile2);
					tileSet.erase(tile3);
				}
			}

			itEnd = cities.end();
			for(it = cities.begin(); it != itEnd; ++it)
			{
				const CornerObjectPtr &object = (*it);
				size_t rolls = 0;

				wxInt32 tile1 = object->tile1().first;
				wxInt32 tile2 = object->tile2().first;
				wxInt32 tile3 = object->tile3().first;

				// See if there's only 1 tile with a roll
				if(-1 != tile<wxInt32>(shRoll, tile1))
				{
					++rolls;
				}

				if(-1 != tile<wxInt32>(shRoll, tile2))
				{
					++rolls;
				}

				if(-1 != tile<wxInt32>(shRoll, tile3))
				{
					++rolls;
				}

				if(1 >= rolls)
				{
					tileSet.erase(tile1);
					tileSet.erase(tile2);
					tileSet.erase(tile3);
				}
			}
		}
	}

	void Rule3(IntSet& tileSet)
	{
		IntSet intersection;

		// Only tiles adjacent to the player's tiles may be removed.
		std::set_intersection(tileSet.begin(), tileSet.end(), 
			mPlayerTiles.begin(), mPlayerTiles.end(), 
			std::inserter(intersection, intersection.begin()));

		tileSet = intersection;
	}

	IntSet mPlayerTiles;
};

IMPLEMENT_RULE(RuleSelectChiplessTile, CHIPLESS_TILES)
