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
// Class: LogicRoadLength
// 
// Determines the longest chain of connected roads (and ships) for the given
// player.
//
// Derived From:
//     <Logic>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
class LogicRoadLength : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		wxASSERT(1 <= input.numItems());

		mPlayer = input.read<wxInt32>();

		// OK, here's how this works.  Build three hashes.  A hash of all
		// sides where the user has a side object, a hash of all "bridge"
		// corners where the user is allowed to transition from one side
		// object to another (i.e. ship to road or vice versa), and a hash
		// of all "dead end" corners, where another player's corner object
		// prevents the road from continuing.
		BuildHashes();

		// Now that the hashes are built, we iterate through each of the side
		// objects, taking it as the starting point.
		SegmentHash::iterator it = mSegmentHash.begin(), 
			itEnd = mSegmentHash.end();

		mCurrentChain.reserve(mSegmentHash.size() / 2);
		
		mLongestLength = 0;

		for(; it != itEnd; ++it)
		{
			// Make sure we haven't already tested this road segment from its
			// other alias.
			wxInt32 thisTile, thisSide;
			boost::tie(thisTile, thisSide) = Utility::decodeSel(it->first);
			wxInt32 otherTile = tile<IntArray>(shSides, thisTile)[thisSide];
			wxInt32 otherID = Utility::encodeSel(otherTile, (thisSide + 3) % 6);

			wxASSERT(mSegmentHash.end() != mSegmentHash.find(otherID));
			if(true == mSegmentHash[otherID].start)
			{
				continue;
			}

			// Reset the current segment length.
			mCurrentLength = 0;

			Traverse(it->first, -1, -1);
			it->second.start = true;

			wxLogDebug(wxT("\n"));
		}

		output = DataObject(mLongestLength, mLongestChain);

		// Clear the chains so we don't hold on to any pointers.
		mDeadEndCorners.clear();
		mBridgeCorners.clear();
		mSegmentHash.clear();
		mLongestChain.clear();
		mCurrentChain.clear();

		playerGameData<wxInt32>(shRoadLength, mPlayer) = mLongestLength;
	}

private:
	struct Segment
	{
		// The object this segment represents.
		SideObjectPtr object;

		// Whether this segment has been counted by the algorithm yet.
		bool counted;

		// Whether this segment has served as a starting point.
		bool start;
	};

	void BuildHashes()
	{
		// First, build the hash of all side objects for this player.
		PlayerGame::SideObjectArray sides;
		playerGame(mPlayer).getAllSideObjects(sides);

		wxLogDebug(wxT("Hash Report %d"), mPlayer);
		wxLogDebug(wxT("-------------"));

		PlayerGame::SideObjectArray::const_iterator it, itEnd = sides.end();
		for(it = sides.begin(); it != itEnd; ++it)
		{
			// While we're in here, reset the longest road flags for each
			// side.
			(*it)->longest(false);

			// Add each to the hash.
			SideObject::TileSide side = (*it)->tile1();
			wxInt32 id = Utility::encodeSel(side.first, side.second);
			mSegmentHash[id].object = (*it);
			mSegmentHash[id].counted = false;
			mSegmentHash[id].start = false;
			wxLogDebug(wxT("%s: Tile %d, Side %d"), 
				HashString(typeid(*(*it)).name()).cwx_str(), 
				side.first, side.second);

			side = (*it)->tile2();
			id = Utility::encodeSel(side.first, side.second);
			mSegmentHash[id].object = (*it);
			mSegmentHash[id].counted = false;
			mSegmentHash[id].start = false;
			wxLogDebug(wxT("%s: Tile %d, Side %d"), 
				HashString(typeid(*(*it)).name()).cwx_str(), 
				side.first, side.second);
		}

		// Now build the sets of all bridge and dead end corners.
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			// If this is the current player, they are bridge corners, 
			// otherwise, dead end corners.
			IntSet &set = (i == mPlayer) ? mBridgeCorners : mDeadEndCorners;
			wxString str = (i == mPlayer) ? swBridge : swBlock;
	
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
					wxLogDebug(wxT("%s %s: Tile %d, Corner %d"), 
						HashString(typeid(*object).name()).cwx_str(), 
						str.c_str(), corner.first, corner.second);

					corner = object->tile2();
					set.insert(Utility::encodeSel(
						corner.first, corner.second));
					wxLogDebug(wxT("%s %s: Tile %d, Corner %d"), 
						HashString(typeid(*object).name()).cwx_str(), 
						str.c_str(), corner.first, corner.second);
					corner = object->tile3();
					set.insert(Utility::encodeSel(
						corner.first, corner.second));
					wxLogDebug(wxT("%s %s: Tile %d, Corner %d"), 
						HashString(typeid(*object).name()).cwx_str(), 
						str.c_str(), corner.first, corner.second);
				}
			}
		}

		wxLogDebug(wxT("\n"));
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
		++mCurrentLength;

		// Here's how it works.  There are four possible branch locations from
		// any invidual segment, two each direction.  We want to check all four
		// branches for another segment.  However, before trying any two 
		// segments, we check to see they are at a dead end corner.  If they 
		// are not, we can check them.  If a segment exists, it can be 
		// traversed, only if it is the same type as the current segment, or
		// if a bridge corner exists between them.
		wxInt32 thisTile, thisSide;
		boost::tie(thisTile, thisSide) = Utility::decodeSel(id);

		wxLogDebug(wxT("LR %d: Length %d, Tile %d, Side %d"), mPlayer,
			mCurrentLength, thisTile, thisSide);

		// Mark this segment as being traversed so we don't hit it again.
		Segment &segment = mSegmentHash[id];
		segment.counted = true;

		// Since each segment is actually in the hash twice, mark the other
		// one as well.
		wxInt32 otherTile = tile<IntArray>(shSides, thisTile)[thisSide];
		wxInt32 otherID = Utility::encodeSel(otherTile, (thisSide + 3) % 6);
		wxASSERT(mSegmentHash.end() != mSegmentHash.find(otherID));
		mSegmentHash[otherID].counted = true;

		// Add the segment to the array of this current chain.
		mCurrentChain.push_back(segment.object);

		// Branch direction 1.
		wxInt32 branch1ID = Utility::encodeSel(thisTile, (thisSide + 1) % 6);
		wxInt32 branch2ID = Utility::encodeSel(otherTile, (thisSide + 2) % 6);

		// Check for a dead end, and avoid going back towards the branch
		// we came from.
		wxInt32 cornerID = branch1ID;
		if( (mDeadEndCorners.end() == mDeadEndCorners.find(cornerID)) &&
			(avoidID1 != branch1ID) &&
			(avoidID1 != branch2ID) &&
			(avoidID2 != branch1ID) &&
			(avoidID2 != branch2ID)
			)
		{
			// No dead end, so we are free to check the branches.
			Branch(id, otherID, branch1ID, segment, cornerID);
			Branch(id, otherID, branch2ID, segment, cornerID);
		}

		// Branch direction 2.
		wxInt32 curSide = (thisSide + 5) % 6;
		branch1ID = Utility::encodeSel(thisTile, curSide);
		otherTile = tile<IntArray>(shSides, thisTile)[curSide];
		branch2ID = Utility::encodeSel(otherTile, (thisSide + 1) % 6);

		// Check for a dead end.
		cornerID = id;
		if( (mDeadEndCorners.end() == mDeadEndCorners.find(cornerID)) &&
			(avoidID1 != branch1ID) &&
			(avoidID1 != branch2ID) &&
			(avoidID2 != branch1ID) &&
			(avoidID2 != branch2ID)
			)
		{
			// No dead end, so we are free to check the branches.
			Branch(id, otherID, branch1ID, segment, cornerID);
			Branch(id, otherID, branch2ID, segment, cornerID);
		}

		// If we've surpassed the longest length, this is the new chain.
		if(mCurrentLength > mLongestLength)
		{
			mLongestChain = mCurrentChain;
			mLongestLength = mCurrentLength;
		}

		// Remove this one from the segment chain.
		mCurrentChain.pop_back();

		// We're off the segment so it can be counted in another chain.
		segment.counted = false;
		mSegmentHash[otherID].counted = false;

		--mCurrentLength;
	}

	wxInt32 mPlayer;
	wxInt32 mCurrentLength;
	wxInt32 mLongestLength;

	WX_DECLARE_HASH_MAP(wxInt32, Segment, wxIntegerHash, wxIntegerEqual,
		SegmentHash);
	SegmentHash mSegmentHash;

	typedef std::set<wxInt32> IntSet;
	IntSet mBridgeCorners;
	IntSet mDeadEndCorners;

	// These containers keep track of which segments make up the longest
	// chain for this player.
	PlayerGame::SideObjectArray mCurrentChain;
	PlayerGame::SideObjectArray mLongestChain;
};

IMPLEMENT_LOGIC(LogicRoadLength, STANDARD)
