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
#include "SideObject.h"
#include "CornerObject.h"
#include "Utility.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicIntriguePlaces
// 
// Determines places the Intrigue card can be played.
//
// Derived From:
//     <Logic>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
class LogicIntriguePlaces : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		// The Intrigue card is playable if any opponent knight touches any
		// road or ship this player owns.

		// First, build a set of all side objects this player owns.  This will
		// cover both road and ship places.
		BuildSidePlacements();

		// Then, go through everyone else's knights, and see if any of them
		// touch anything in the side placements.  If they do, they can
		// be displaced with the Intrigue card.
		BuildAvailablePlacements();

		output = DataObject(mAvailablePlacements);

		mSidePlacements.clear();
		mAvailablePlacements.clear();
	}

private:
	typedef std::pair<wxInt32, wxInt32> TileCorner;
	typedef std::set<TileCorner> TileCornerSet;
	typedef std::set<wxInt32> IntSet;

	void BuildSidePlacements()
	{
		PlayerGame::SideObjectArray objects;
		playerGame().getAllSideObjects(objects);

		SideObject::TileSide tile1, tile2;

		// Go through them and create entries for each one.
		PlayerGame::SideObjectArray::const_iterator it, itEnd = objects.end();
		for(it = objects.begin(); it != itEnd; ++it)
		{
			const SideObjectPtr& object = (*it);
			tile1 = object->tile1();
			tile2 = object->tile2();

			wxInt32 id1 = Utility::encodeSel(tile1.first, tile1.second);
			wxInt32 id2 = Utility::encodeSel(tile2.first, tile2.second);

			mSidePlacements.insert(id1);
			mSidePlacements.insert(id2);
		}
	}

	void BuildAvailablePlacements()
	{
		wxInt32 curPlayer = current();
		PlayerGame::CornerObjectArray objects;

		// Go through each player and add their knights to the big list of
		// knights to check.
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			if(i != curPlayer)
			{
				const PlayerGame::CornerObjectArray& knights =
					playerGame(i).getCornerObjects(shKnights);
				std::copy(knights.begin(), knights.end(), 
					std::back_inserter(objects));
			}
		}

		CornerObject::TileCorner tile1;
		wxInt32 side1, side2, side3;

		// Now go through each knight and look for the three sides it touches
		// in the side placements.
		PlayerGame::CornerObjectArray::const_iterator it, 
			itEnd = objects.end();
		for(it = objects.begin(); it != itEnd; ++it)
		{
			const CornerObjectPtr& knight = (*it);
			tile1 = knight->tile1();
			wxInt32 thisTile = tile1.first;
			wxInt32 corner = tile1.second;

			wxInt32 otherTile = tile<IntArray>(shSides, thisTile)[corner];
			wxInt32 otherCorner = (corner + 4) % 6;

			// Build the three side ids from this tile.
			side1 = Utility::encodeSel(thisTile, corner);
			side2 = Utility::encodeSel(thisTile, (corner + 5) % 6);
			side3 = Utility::encodeSel(otherTile, otherCorner);

			if( (mSidePlacements.end() != mSidePlacements.find(side1)) ||
				(mSidePlacements.end() != mSidePlacements.find(side2)) ||
				(mSidePlacements.end() != mSidePlacements.find(side3)))
			{
				mAvailablePlacements.push_back(knight);
			}
		}
	}

	IntSet mSidePlacements;
	PlayerGame::CornerObjectArray mAvailablePlacements;
};

IMPLEMENT_LOGIC(LogicIntriguePlaces, CITIES)
