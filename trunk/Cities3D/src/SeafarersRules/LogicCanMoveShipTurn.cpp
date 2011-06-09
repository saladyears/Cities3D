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
#include "HexObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicCanMoveShipTurn
// 
// Determines whether a ship can be moved during the player's turn.
//
// Derived From:
//     <Logic>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <Seafarers>
//
// Logic Called:
//
class LogicCanMoveShipTurn : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		// These are the conditions that must be met for a ship to be able to
		// be moved:
		//
		// 1)  No other ships may have been moved in this turn.
		// 2)  The player must have a ship on the board.
		// 3)  The ship must not have been built in this turn.
		// 4)  The ship must not border the pirate tile.
		// 5)  The ship must not be between two other ships of the same color 
		//     or a ship and a city/settlement of the same color.
		// 6)  The player must have a valid place to move the ship.

		const HexObjectPtr& pirate = GetGame()->getHexObject(shPirate);
		wxASSERT(pirate);

		wxInt32 pirateTile = pirate->tile();
		wxInt32 thisTurn = gameData<wxInt32>(shTurn);

		PlayerGame::SideObjectArray valid;

		// Conditions 1 and 2.
		PlayerGame::SideObjectArray ships = 
			playerGame().getSideObjects(shShips);

		if( (FALSE == playerGameData<wxInt32>(shMovedShip)) &&
			(0 < ships.size()))
		{
			BuildSets();

			PlayerGame::SideObjectArray::const_iterator it, 
				itEnd = ships.end();
			for(it = ships.begin(); it != itEnd; ++it)
			{
				SideObjectPtr object = (*it);

				SideObject::TileSide tile1 = object->tile1();
				SideObject::TileSide tile2 = object->tile2();

				// Conditions 3 and 4.
				if( (thisTurn != object->turn()) &&
					(tile1.first != pirateTile) &&
					(tile2.first != pirateTile))
				{
					// Condition 5.
					if(true == ExamineMoves(tile1))
					{
						// Conditon 6.

						// Test to see if we have a place to move the ship
						// by temporarily removing it from the player's ship
						// list and running the logic that determines if
						// a new ship could be placed.  If the only location
						// returned is the original location, the cannot move
						// the ship.
						playerGame().removeSideObject(shShips, object);

						DataObject input(0), output;
						RULE.Decide(shLogicCanPlaceShip, input, output);

						// They must have more than the original 
						// placements.
						TileSideSet sides = output.read<TileSideSet>();
						playerGame().addSideObject(shShips, object);

						if(1 < sides.size())
						{
							valid.push_back(object);
						}
					}
				}
			}
		}

		mCornerSet.clear();
		mSideSet.clear();

		bool canMove = false;
		
		if(false == valid.empty())
		{
			canMove = true;
		}

		output = DataObject(canMove, 1, valid);
	}

private:
	void BuildSets()
	{
		// Fill our sets with all ships the player has, and all corner
		// objects the player has.
		const PlayerGame::SideObjectArray& ships = 
			playerGame().getSideObjects(shShips);

		PlayerGame::SideObjectArray::const_iterator it, 
				itEnd = ships.end();
		for(it = ships.begin(); it != itEnd; ++it)
		{
			const SideObjectPtr &object = (*it);

			mSideSet.insert(object->tile1());
			mSideSet.insert(object->tile2());
		}

		// This is kind of cheating in that it will take care of knights as
		// well as they close off shipping lanes.
		PlayerGame::CornerObjectArray objects;
		playerGame().getAllCornerObjects(objects);

		{
			PlayerGame::CornerObjectArray::const_iterator it, 
				itEnd = objects.end();
			for(it = objects.begin(); it != itEnd; ++it)
			{
				const CornerObjectPtr &object = (*it);
				
				mCornerSet.insert(object->tile1());
				mCornerSet.insert(object->tile2());
				mCornerSet.insert(object->tile3());
			}
		}
	}

	bool ExamineMoves(SideObject::TileSide& tileSide)
	{
		bool left = false;
		bool right = false;

		wxInt32 thisTile = tileSide.first;
		wxInt32 thisSide = tileSide.second;

		// Condition 5: Verify that the ship is not between two others ships
		// or a ship and a corner object.

		wxInt32 oppTile = tile<IntArray>(shSides, thisTile)[thisSide];
		wxASSERT(-1 != oppTile);

		// Check "left" side for a ship or a corner object.
		SideObject::TileSide tile1 = std::make_pair(thisTile, 
			(thisSide + 1) % 6);
		SideObject::TileSide tile2 = std::make_pair(oppTile, 
			(thisSide + 2) % 6);
		CornerObject::TileCorner corner = std::make_pair(thisTile, 
			(thisSide + 1) % 6);

		if( (mSideSet.end() != mSideSet.find(tile1)) ||
			(mSideSet.end() != mSideSet.find(tile2)) ||
			(mCornerSet.end() != mCornerSet.find(corner)))
		{
			left = true;
		}

		// Check "right" side for a ship or a corner object.
		tile1 = std::make_pair(thisTile, (thisSide + 5) % 6);
		tile2 = std::make_pair(oppTile, (thisSide + 4) % 6);
		corner = std::make_pair(thisTile, thisSide);

		if( (mSideSet.end() != mSideSet.find(tile1)) ||
			(mSideSet.end() != mSideSet.find(tile2)) ||
			(mCornerSet.end() != mCornerSet.find(corner)))
		{
			right = true;
		}

		// There's no way a ship could be out there by itself.
		wxASSERT(left || right);

		bool canMove = false;

		if( (false == left) ||
			(false == right))
		{
			canMove = true;
		}

		return canMove;
	}

	typedef std::set<CornerObject::TileCorner> TileCornerSet;
	typedef std::set<SideObject::TileSide> TileSideSet;

	TileCornerSet mCornerSet;
	TileSideSet mSideSet;
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCanMoveShipTurn, Turn,
							LogicCanMoveShip, SEAFARERS)
