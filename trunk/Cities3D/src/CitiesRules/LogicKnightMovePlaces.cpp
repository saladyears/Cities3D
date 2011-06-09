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
#include "KnightObject.h"
#include "Utility.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicKnightMovePlaces
// 
// Determines locations, if any, a knight may move to (or attack).
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
class LogicKnightMovePlaces : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		TileCorner knight = input.read<TileCorner>();
		mPlayer = input.read<wxInt32>(1);

		bool attack = false;
		wxInt32 strength = -1;

		// Check for the attack flag.
		if(3 <= input.numItems())
		{
			attack = true;
			strength = input.read<wxInt32>(2);
		}

		TileCornerSet placements;

		// If we're attacking another knight, we need to build the set of all
		// opponent knights that could possibly be attacked by this knight.
		if(true == attack)
		{
			BuildAttackPlacements(strength, placements);
		}
		// Otherwise, if we're moving the knight, build the set of all valid
		// move locations (which is just where can a new knight be placed).
		else
		{
			RULE.Decide(shLogicCanPlaceKnight, DataObject(mPlayer), output);
			placements = output.read<TileCornerSet>();
		}

		if(false == placements.empty())
		{
			BuildPossiblePlacements(knight, placements);

			BuildPlacements(knight);
		}

		output = DataObject(mAvailablePlacements);

		mAvailablePlacements.clear();
		mSegmentHash.clear();
		mPossiblePlacements.clear();
		mBridgeCorners.clear();
		mDeadEndCorners.clear();
	}

private:
	typedef std::pair<wxInt32, wxInt32> TileCorner;
	typedef std::set<TileCorner> TileCornerSet;

	struct Segment
	{
		SideObjectPtr object;

		bool counted;
	};

	void BuildAttackPlacements(wxInt32 strength, TileCornerSet& placements)
	{
		// Gather up all of the knights that are below this knight in strength.
		// If any exist on the board, they are potential attack victims.
		wxInt32 players = numPlayers();
		for(wxInt32 i = 0; i < players; ++i)
		{
			// Can't attack self.
			if(i == mPlayer)
			{
				continue;
			}

			const PlayerGame::CornerObjectArray& knights =
				playerGame(i).getCornerObjects(shKnights);
			PlayerGame::CornerObjectArray::const_iterator it, 
				itEnd = knights.end();

			for(it = knights.begin(); it != itEnd; ++it)
			{
				const KnightObject* knight = static_cast<KnightObject*>(
					(*it).get());

				if(knight->strength() < strength)
				{
					placements.insert(knight->tile1());
					placements.insert(knight->tile2());
					placements.insert(knight->tile3());
				}
			}
		}
	}

	void BuildPossiblePlacements(const TileCorner& knight,
		const TileCornerSet& placements)
	{
		TileCorner knight1 = knight;
		TileCorner knight2 = TileCorner(
			tile<IntArray>(shSides, knight.first)[knight.second],
			(knight.second + 4) % 6);
		TileCorner knight3 = TileCorner(
			tile<IntArray>(shSides, knight.first)[(knight.second + 5) % 6],
			(knight.second + 2) % 6);

		// Convert this set of placements into the ids we will look for later.
		// Skip the corner that the knight is already on as we go through.
		TileCornerSet::const_iterator it, itEnd = placements.end();
		for(it = placements.begin(); it != itEnd; ++it)
		{
			const TileCorner& tileCorner = (*it);
			if( (tileCorner != knight1) &&
				(tileCorner != knight2) &&
				(tileCorner != knight3))
			{
				// Add the corner as all three possibilities.
				wxInt32 tile1 = tileCorner.first;
				wxInt32 corner1 = tileCorner.second;
				wxInt32 id = Utility::encodeSel(tile1, corner1);

				mPossiblePlacements.insert(id);

				wxInt32 tile2 = tile<IntArray>(shSides, tile1)[corner1];
				wxInt32 corner2 = (corner1 + 4) % 6;
				id = Utility::encodeSel(tile2, corner2);

				mPossiblePlacements.insert(id);

				wxInt32 tile3 = 
					tile<IntArray>(shSides, tile1)[(corner1 + 5) % 6];
				wxInt32 corner3 = (corner1 + 2) % 6;
				id = Utility::encodeSel(tile3, corner3);

				mPossiblePlacements.insert(id);
			}
		}
	}

	void BuildPlacements(const TileCorner& knight)
	{
		BuildHashes();

		// Now that we've got our hashes set up, start from the knight's corner
		// and search side objects, looking to see if any corners match any
		// inside mAllPlacements.
		CheckCorners(knight);
	}

	void BuildHashes()
	{
		// Just like longest road, build a set of blocked corners, and a set
		// of bridge corners.
		// First, build the hash of all side objects for this player.
		PlayerGame::SideObjectArray sides;
		playerGame(mPlayer).getAllSideObjects(sides);

		PlayerGame::SideObjectArray::const_iterator it, itEnd = sides.end();
		for(it = sides.begin(); it != itEnd; ++it)
		{
			// Add each to the hash.
			SideObject::TileSide side = (*it)->tile1();
			wxInt32 id = Utility::encodeSel(side.first, side.second);
			mSegmentHash[id].object = (*it);
			mSegmentHash[id].counted = false;
			
			side = (*it)->tile2();
			id = Utility::encodeSel(side.first, side.second);
			mSegmentHash[id].object = (*it);
			mSegmentHash[id].counted = false;
		}

		// Now build the sets of all bridge and dead end corners.
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			// If this is the current player, they are bridge corners, 
			// otherwise, dead end corners.
			IntSet &set = (i == mPlayer) ? mBridgeCorners : mDeadEndCorners;
			
			PlayerGame::CornerObjectArray objects;
			playerGame(i).getAllCornerObjects(objects);

			PlayerGame::CornerObjectArray::const_iterator it, 
				itEnd = objects.end();

			for(it = objects.begin(); it != itEnd; ++it)
			{
				// Only add them if they are dead ends, or bridges with the
				// bridge flag set.
				const CornerObjectPtr& object = (*it);

				if( (i != mPlayer) ||
					(true == object->bridge()))
				{
					CornerObject::TileCorner corner = object->tile1();
					set.insert(Utility::encodeSel(
						corner.first, corner.second));
					corner = object->tile2();
					set.insert(Utility::encodeSel(
						corner.first, corner.second));
					corner = object->tile3();
					set.insert(Utility::encodeSel(
						corner.first, corner.second));
				}
			}
		}
	}

	void CheckCorners(const TileCorner& knight)
	{
		// There are three possible road/ship options for this object.  Iterate
		// on each of them.
		wxInt32 tile1 = knight.first;
		wxInt32 corner1 = knight.second;

		wxInt32 id1 = Utility::encodeSel(tile1, corner1);
		wxInt32 id2 = Utility::encodeSel(tile1, (corner1 + 5) % 6);
		wxInt32 tile2 = tile<IntArray>(shSides, tile1)[corner1];
		wxInt32 id3 = Utility::encodeSel(tile2, (corner1 + 4) % 6);

		Traverse(id1, id2, id3);
		Traverse(id2, id1, id3);
		Traverse(id3, id1, id2);
	}

	void Branch(wxInt32 currentID, wxInt32 otherID, wxInt32 branchID, 
		Segment &segment, wxInt32 cornerID)
	{
		// See if the branch exists.  If it does, and it differs in type from 
		// the current segment, there has to be a bridge corner, otherwise, 
		// we're free to traverse.
		SegmentHash::iterator it = mSegmentHash.find(branchID);
		if( (mSegmentHash.end() != it) &&
			(false == it->second.counted) &&
			((typeid(*segment.object) == typeid(*(it->second.object))) ||
			(mBridgeCorners.end() != mBridgeCorners.find(cornerID)))
			)
		{
			Traverse(branchID, currentID, otherID);
		}
	}

	void Traverse(wxInt32 id, wxInt32 avoidID1, wxInt32 avoidID2)
	{
		SegmentHash::iterator it = mSegmentHash.find(id);

		// If there's no side object here, or it has already been visited,
		// do nothing.
		if( (mSegmentHash.end() == it) ||
			(true == it->second.counted))
		{
			return;
		}

		// Mark this segment as being traversed so we don't hit it again.
		Segment &segment = it->second;
		segment.counted = true;

		// In addition, mark the other side as being visited.
		wxInt32 thisTile, thisSide;
		boost::tie(thisTile, thisSide) = Utility::decodeSel(id);

		wxInt32 otherTile = tile<IntArray>(shSides, thisTile)[thisSide];
		wxInt32 otherID = Utility::encodeSel(otherTile, (thisSide + 3) % 6);
		wxASSERT(mSegmentHash.end() != mSegmentHash.find(otherID));
		mSegmentHash[otherID].counted = true;

		// Since there is a side object here, do two things.  1) Look to see
		// if we hit one of the potential move corners, and 2) Continue to
		// look for more side objects to traverse.

		// Branch direction 1.
		wxInt32 branch1ID = Utility::encodeSel(thisTile, (thisSide + 1) % 6);
		wxInt32 branch2ID = Utility::encodeSel(otherTile, (thisSide + 2) % 6);
		wxInt32 cornerID = branch1ID;

		if( (avoidID1 != branch1ID) &&
			(avoidID1 != branch2ID) &&
			(avoidID2 != branch1ID) &&
			(avoidID2 != branch2ID)
			)
		{
			// See if we found a corner.
			if(mPossiblePlacements.end() != mPossiblePlacements.find(cornerID))
			{
				wxInt32 foundTile, foundCorner;
				boost::tie(foundTile, foundCorner) = 
					Utility::decodeSel(cornerID);
				mAvailablePlacements.insert(
					TileCorner(foundTile, foundCorner));
			}

			// No dead end, so we are free to check the branches.
			if(mDeadEndCorners.end() == mDeadEndCorners.find(cornerID))
			{
				Branch(id, otherID, branch1ID, segment, cornerID);
				Branch(id, otherID, branch2ID, segment, cornerID);
			}
		}

		// Branch direction 2.
		wxInt32 curSide = (thisSide + 5) % 6;
		branch1ID = Utility::encodeSel(thisTile, curSide);
		otherTile = tile<IntArray>(shSides, thisTile)[curSide];
		branch2ID = Utility::encodeSel(otherTile, (thisSide + 1) % 6);

		cornerID = id;

		if( (avoidID1 != branch1ID) &&
			(avoidID1 != branch2ID) &&
			(avoidID2 != branch1ID) &&
			(avoidID2 != branch2ID)
			)
		{
			// See if we found a corner.
			if(mPossiblePlacements.end() != mPossiblePlacements.find(cornerID))
			{
				wxInt32 foundTile, foundCorner;
				boost::tie(foundTile, foundCorner) = 
					Utility::decodeSel(cornerID);
				mAvailablePlacements.insert(
					TileCorner(foundTile, foundCorner));
			}

			// No dead end, so we are free to check the branches.
			if(mDeadEndCorners.end() == mDeadEndCorners.find(cornerID))
			{
				Branch(id, otherID, branch1ID, segment, cornerID);
				Branch(id, otherID, branch2ID, segment, cornerID);
			}
		}

		// We're off the segment so it can be counted in another chain.
		segment.counted = false;
		mSegmentHash[otherID].counted = false;
	}

	wxInt32 mPlayer;

	TileCornerSet mAvailablePlacements;

	WX_DECLARE_HASH_MAP(wxInt32, Segment, wxIntegerHash, wxIntegerEqual,
		SegmentHash);

	SegmentHash mSegmentHash;

	typedef std::set<wxInt32> IntSet;
	IntSet mPossiblePlacements;
	IntSet mBridgeCorners;
	IntSet mDeadEndCorners;
};

IMPLEMENT_LOGIC(LogicKnightMovePlaces, CITIES)
