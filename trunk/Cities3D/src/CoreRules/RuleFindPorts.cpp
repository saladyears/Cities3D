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
#include "GameEngine.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	wxInt32 portIterateNext(const GamePtr &game, wxInt32 thisTile, 
		wxInt32 side, wxInt32 &length)
	{
		wxInt32 curLength;
		wxInt32 nextTile;

		DataObject input(thisTile), output;
		RULE.Decide(shLogicTileIsOcean, input, output);
		
		//if this is an ocean, stop counting
		if(true == output.read<bool>())
		{
			return length;
		}

		//get the next tile
		nextTile = game->tile(thisTile)->read<IntArray>(shSides)[side];

		//sanity
		if(-1 == nextTile)
		{
			return length;
		}

		//increment the length for getting to a new non-ocean tile
		length++;

		//keep going down this path
		curLength = portIterateNext(game, nextTile, side, length);

		//move back off this tile
		length--;

		//return the longest we got
		return curLength;
	}

	//this algorithm works to find the side direction where the longest
	//length of land tiles lays for this particular port tile; on the standard
	//maps, this is the mechanism to determine which they they should face, 
	//and on other maps it is a good default mechanism; maps can override this
	//algorithm by explicitly setting the PortSide variable for a TileObject 
	//object
	wxInt32 portIterateStart(const GamePtr &game, wxInt32 thisTile)
	{
		wxInt32 length = 0;
		wxInt32 curLength = 0;
		wxInt32 side = 0;
		
		//run through each side and determine if we can iterate down it
		for(wxInt32 i = 0; i < 6; i++)
		{
			//get the next tile
			wxInt32 nextTile = 
				game->tile(thisTile)->read<IntArray>(shSides)[i];

			//sanity
			if(-1 == nextTile)
			{
				continue;
			}

			wxInt32 temp = portIterateNext(game, nextTile, i, length);

			//set the side if needed
			if(temp > curLength)
			{
				side = i;
				curLength = temp;
			}
		}

		return side;
	}
}

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleFindPorts
// 
// Finds all port <TileObjects> on the board.  Stores their indices
// in an array in the <Game> data for easy lookup in the future.
// Orients the <TileObjects> properly so the ports face the proper
// land tile.  Sets the two indices on the <TileObject> that are the
// corners a port settlement/city may be built on.
//
// Derived From:
//     <Rule>
//
// Project:
//     <CoreRules>
//
// RuleSet:
//     <Core>
//
// Logic Called:
//     <LogicTileIsPort>
//

class RuleFindPorts : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		//create the array of all ports on the board
		IntArray &ports(gameData<IntArray>(shPorts));

		//clear current port data
		ports.clear();

		//run through the list
		for(wxInt32 i = 0; i < numTiles(); i++)
		{
			DataObject input(i), output;
			RULE.Decide(shLogicTileIsPort, input, output);
			
			if(true == output.read<bool>())
			{
				ports.push_back(i);
			}
		}

		//now that we've got ports, we need to rotate them and set the board 
		//port corners correctly
		IntArray::const_iterator it;
		for(it = ports.begin(); it != ports.end(); ++it)
		{
			wxInt32 thisTile = (*it);

			//now calculate which side the tile should face, if it does not 
			//have a permanent setting
			wxInt32 side;
			side = tile<wxInt32>(shPortSide, thisTile);

			//if it's not set, use the recursive algorithm to determine which 
			//way it should face
			if(-1 == side)
			{
				side = portIterateStart(GetGame(), thisTile);
			}

			//set the port values
			tile<wxInt32>(shPort1, thisTile) = side;
			tile<wxInt32>(shPort2, thisTile) = (side + 1) % 6;

			//set the rotation
			Vector rotation(0.0, ((side + 4) % 6) * 60., 0.0);
			tile(thisTile)->rotation(rotation);
		}
	}
};

IMPLEMENT_RULE(RuleFindPorts, CORE);
