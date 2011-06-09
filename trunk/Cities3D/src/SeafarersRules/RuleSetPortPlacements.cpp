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
#include "UtilityMap.h"
#include "GameEngine.h"
#include "Map.h"
#include "Controller.h"
#include "ConfigDatabase.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleSetPortPlacements
// 
// Reveals all user-placed ports.  Traverses the map,
// finding all tiles with a TileType of PortRandom, and
// setting the tile's TileType to a new randomly selected
// port type from the map's RandomPorts IntHash.  Reorders all
// players randomly.  Sets the current player to the (new) 
// first player.  Removes no longer needed fields from the
// <Game> data.  Updates the <PlayerUIs> to refresh themselves
// according to the new player order.
//
// Derived From:
//     <Rule>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <PortPlacement>
//
// Rules Called:
//     <RuleRandomizePlayers>
//
// Transmits To:
//     eventPlayerReorder
//
class RuleSetPortPlacements : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		HashStringArray values;

		UtilityMap::prepareDataArray(MAP.read<Data::IntHash>(
			shRandomPorts), values);

		//now iterate through the tiles, finding all marked as RandomPort, and
		//set them to the randomized value
		size_t j = 0;
		for(wxInt32 i = 0; i < numTiles(); ++i)
		{
			const HashString& type = tile<HashString>(shTileType, i);

			if(shPortRandom == type)
			{
				tile<HashString>(shTileType, i) = values[j++];
			}
		}

		//sanity
		wxASSERT(values.size() == j);

		//clean up values no longer needed
		eraseGameData<wxInt32>(shInitialPortTile);
		eraseGameData<wxInt32>(shInitialPortCount);

		//restart at the top
		gameData<wxInt32>(shCurrentPlayer) = 0;

		//now re-randomize all of the players
		RULE.Execute(shRuleRandomizePlayers, DataObject(-1));

		//finally, gut the players in the UI
		Controller::get().Transmit(shEventPlayerReorder, GetGame());
	}
};

IMPLEMENT_RULE(RuleSetPortPlacements, PORT_PLACEMENT)
