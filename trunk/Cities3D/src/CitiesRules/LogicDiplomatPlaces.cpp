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
// Class: LogicDiplomatPlaces
// 
// Determines places the Diplomat card can be played.
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
class LogicDiplomatPlaces : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		PlayerGame::SideObjectArray availablePlacements;

		// The algorithm works this way:
		//
		// 1.  Iterate over each player.
		// 2.  Build a set of all CornerObject locations and SideObject
		//     locations for the player.
		// 3.  For each SideObject, if it is bounded on both sides by other
		//     SideObjects of the same type, or on one side, and bounded by
		//     a CornerObject on the other, it may not be moved.
		wxInt32 players = numPlayers();
		for(wxInt32 i = 0; i < players; ++i)
		{
			BuildAvailablePlacements(i, availablePlacements);
		}

		output = DataObject(availablePlacements);
	}

private:
	typedef std::set<wxInt32> IntSet;
	typedef std::map<wxInt32, SideObjectPtr> SideMap;
		
	void BuildAvailablePlacements(wxInt32 player, 
		PlayerGame::SideObjectArray& availablePlacements)
	{
		// Build a set of closed corners.
		IntSet closedCorners;
		BuildClosedCorners(player, closedCorners);

		// Build a set of closed sides.
		SideMap closedSides;
		BuildClosedSides(player, closedSides);

		// Now iterate through all SideObjects for the player and test.
		PlayerGame::SideObjectArray objects;
		playerGame(player).getAllSideObjects(objects);

		PlayerGame::SideObjectArray::const_iterator it,
			itEnd = objects.end();
		for(it = objects.begin(); it != itEnd; ++it)
		{
			const SideObjectPtr& object = (*it);

			// Look for a blocking side object or corner object on each "side".
			if(true == ExamineSide(object, closedCorners, closedSides))
			{
				availablePlacements.push_back(object);
			}
		}
	}

	void BuildClosedCorners(wxInt32 player, IntSet& closed)
	{
		PlayerGame::CornerObjectArray objects;
		playerGame(player).getAllCornerObjects(objects);

		CornerObject::TileCorner tile1, tile2, tile3;

		PlayerGame::CornerObjectArray::const_iterator it,
			itEnd = objects.end();
		for(it = objects.begin(); it != itEnd; ++it)
		{
			const CornerObjectPtr& object = (*it);
			tile1 = object->tile1();
			tile2 = object->tile2();
			tile3 = object->tile3();
			
			closed.insert(Utility::encodeSel(tile1.first, tile1.second));
			closed.insert(Utility::encodeSel(tile2.first, tile2.second));
			closed.insert(Utility::encodeSel(tile3.first, tile3.second));
		}
	}

	void BuildClosedSides(wxInt32 player, SideMap& closed)
	{
		PlayerGame::SideObjectArray objects;
		playerGame(player).getAllSideObjects(objects);

		SideObject::TileSide tile1, tile2;

		PlayerGame::SideObjectArray::const_iterator it,
			itEnd = objects.end();
		for(it = objects.begin(); it != itEnd; ++it)
		{
			const SideObjectPtr& object = (*it);
			tile1 = object->tile1();
			tile2 = object->tile2();
			
			closed[Utility::encodeSel(tile1.first, tile1.second)] = object;
			closed[Utility::encodeSel(tile2.first, tile2.second)] = object;
		}
	}

	bool ExamineSide(const SideObjectPtr& object, IntSet& corners, 
		SideMap& sides)
	{
		bool left = false;
		bool right = false;
		bool canMove = false;

		SideObject::TileSide tileSide = object->tile1();
		wxInt32 thisTile = tileSide.first;
		wxInt32 thisSide = tileSide.second;

		wxInt32 oppTile = tile<IntArray>(shSides, thisTile)[thisSide];
		wxASSERT(-1 != oppTile);

		// Check "left" side for a blocking side or a corner object.
		wxInt32 tile1 = Utility::encodeSel(thisTile, (thisSide + 1) % 6);
		wxInt32 tile2 = Utility::encodeSel(oppTile, (thisSide + 2) % 6);
		wxInt32 corner = Utility::encodeSel(thisTile, (thisSide + 1) % 6);

		SideMap::iterator it;
		SideObjectPtr side;
		it = sides.find(tile1);
		if(sides.end() == it)
		{
			it = sides.find(tile2);
		}
		if(sides.end() != it)
		{
			side = it->second;
		}

		if( (corners.end() != corners.find(corner)) ||
			(side && typeid(*side) == typeid(*object)))
		{
			left = true;
		}

		// Check "right" side for a ship or a corner object.
		tile1 = Utility::encodeSel(thisTile, (thisSide + 5) % 6);
		tile2 = Utility::encodeSel(oppTile, (thisSide + 4) % 6);
		corner = Utility::encodeSel(thisTile, thisSide);

		side = SideObjectPtr();
		it = sides.find(tile1);
		if(sides.end() == it)
		{
			it = sides.find(tile2);
		}
		if(sides.end() != it)
		{
			side = it->second;
		}

		if( (corners.end() != corners.find(corner)) ||
			(side && typeid(*side) == typeid(*object)))
		{
			right = true;
		}

		// There must always be at least one left or right side connection.
		wxASSERT(left || right);

		if( (false == left) || 
			(false == right))
		{
			canMove = true;
		}

		return canMove;
	}
};

IMPLEMENT_LOGIC(LogicDiplomatPlaces, CITIES)
