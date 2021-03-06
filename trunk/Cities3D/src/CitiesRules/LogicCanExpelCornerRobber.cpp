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
#include "HexObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicCanExpelCornerRobber
// 
// Determines whether the robber borders the given tile..
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
// Logic Called:
//
class LogicCanExpelCornerRobber : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		typedef std::pair<wxInt32, wxInt32> TileCorner;
		const TileCorner& thisTile = input.read<TileCorner>();
		wxInt32 tile1 = thisTile.first;
		wxInt32 corner1 = thisTile.second;

		// Grab which tile the robber is on.
		const HexObjectPtr& robber = GetGame()->getHexObject(shRobber);
		wxInt32 robberTile = robber->tile();

		// If the robber is hidden, it is on no tile.
		if(true == robber->isset(IGameObject::Hidden))
		{
			robberTile = -1;
		}

		bool border = true;

		if(robberTile != tile1)
		{
			// Try the second tile.
			wxInt32 tile2 = tile<IntArray>(shSides, tile1)[corner1];
			if(robberTile != tile2)
			{
				// Try the third.
				wxInt32 tile3 = 
					tile<IntArray>(shSides, tile1)[(corner1 + 5) % 6];
				if(robberTile != tile3)
				{
					border = false;
				}
			}
		}

		output = DataObject(border);
	}
};

IMPLEMENT_LOGIC_MIXIN(LogicCanExpelCornerRobber, LogicCanExpelCorner, CITIES);
