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
#include "CornerObject.h"
#include "IRuleEngine.h"
#include "SideObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicIsValidKnightPlacement
// 
// Determines if a knight placement is a valid placement.  As long as there
// is a side object belonging to the user that touches the corner, it is a
// valid placement.
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
//
class LogicIsValidKnightPlacement : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		bool valid = false;

		const CornerObject::TileCorner &thisTile = 
			input.read<CornerObject::TileCorner>();
		wxInt32 player = input.read<wxInt32>(1);

		const wxInt32 index = thisTile.first;
		const wxInt32 corner = thisTile.second;

		// Look for side objects touching this corner.
		PlayerGame::SideObjectArray objects;
		playerGame(player).getAllSideObjects(objects);
		
		// There are six tile/side possibilities for an object to end at this
		// corner.  Create them here once outside the loop.
		typedef std::pair<wxInt32, wxInt32> TileSide;
		typedef std::vector<TileSide> TileSideVector;

		TileSideVector sides;
		sides.reserve(6);

		// Side 1.
		wxInt32 curTile = index;
		wxInt32 curSide = corner;

		sides.push_back(TileSide(curTile, curSide));
		sides.push_back(TileSide(tile<IntArray>(shSides, curTile)[curSide],
			(curSide + 3) % 6));

		// Side 2.
		curSide = (corner + 5) % 6;
		sides.push_back(TileSide(curTile, curSide));
		sides.push_back(TileSide(tile<IntArray>(shSides, curTile)[curSide],
			(curSide + 3) % 6));

		// Side 3 (both neighboring tiles).
		curTile = tile<IntArray>(shSides, index)[corner];
		curSide = (corner + 4) % 6;
		sides.push_back(TileSide(curTile, curSide));
		sides.push_back(TileSide(tile<IntArray>(shSides, curTile)[curSide],
			(curSide + 3) % 6));

		PlayerGame::SideObjectArray::const_iterator it, itEnd = objects.end();
		for(it = objects.begin(); it != itEnd; ++it)
		{
			// See if any of these objects end at the corner.
			if( (sides.end() != std::find(sides.begin(), sides.end(), 
				(*it)->tile1())) ||
				(sides.end() != std::find(sides.begin(), sides.end(), 
				(*it)->tile2()))
				)
			{
				valid = true;
				break;
			}
		}

		output = DataObject(valid);
	}
};

IMPLEMENT_LOGIC(LogicIsValidKnightPlacement, CITIES);
