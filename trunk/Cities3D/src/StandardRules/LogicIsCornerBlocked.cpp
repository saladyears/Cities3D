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

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicIsCornerBlocked
// 
// Searches for objects of the given type within one corner of the given 
// corner.  If any objects exist, the given corner is blocked and cannot be
// built on.
//
// Derived From:
//     <LogicBooleanMixin>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
class LogicIsCornerBlocked : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		wxASSERT(2 <= input.numItems());

		typedef std::pair<wxInt32, wxInt32> TileCorner;
		const HashString &type = input.read<HashString>();
		const TileCorner &thisTile = input.read<TileCorner>(1);

		// Grabe the other tile to compare against.
		wxInt32 corner = thisTile.second;
		wxInt32 otherTile = tile<IntArray>(shSides, 
			thisTile.first)[corner];

		// Create an array of corners to check against.
		typedef std::vector<TileCorner> CornerArray;
		CornerArray corners;
		corners.reserve(4);
		corners.push_back(thisTile);
		corners.push_back(TileCorner(thisTile.first, (corner + 1) % 6));
		corners.push_back(TileCorner(thisTile.first, (corner + 5) % 6));
		corners.push_back(TileCorner(otherTile, (corner + 5) % 6));

		// Now look through all players and see if any corner objects of the
		// given type are on any of the corners.
		bool blocked = false;

		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			const PlayerGame::CornerObjectArray &objects = 
				playerGame(i).getCornerObjects(type);

			PlayerGame::CornerObjectArray::const_iterator it, 
				itEnd = objects.end();
			for(it = objects.begin(); it != itEnd; ++it)
			{
				CornerArray::const_iterator it2, it2End = corners.end();
				for(it2 = corners.begin(); it2 != it2End; ++it2)
				{
					if(true == (*it)->onTileCorner((*it2)))
					{
						blocked = true;
						break;
					}
				}
			}
		}

		output = DataObject(blocked);
	}
};

IMPLEMENT_LOGIC(LogicIsCornerBlocked, STANDARD);
