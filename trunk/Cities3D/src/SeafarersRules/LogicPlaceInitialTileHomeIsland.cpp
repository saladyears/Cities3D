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
#include "GameEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicPlaceInitialTileHomeIsland
// 
// Prevents initial placements from being placed on anything
// but the player's home island.  If the player does not yet
// have a home island, prevents the player from placing on a
// home island that already has half of the number of players
// on it.
//
// Derived From:
//     <Logic>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <TradeRoutes>
//
// Mixin To:
//     <LogicPlaceInitialTile>
//
// Logic Called:
//     <LogicTileIsLand>
//
class LogicPlaceInitialTileHomeIsland : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		wxASSERT(1 <= input.numItems());

		wxInt32 thisTile = input.read<wxInt32>();
		wxInt32 tileIsland = tile<wxInt32>(shIsland, thisTile);
		wxInt32 playerIsland = playerGameData<wxInt32>(shHomeIsland);

		//if the player has already selected an island, this tile must
		//be on the same island
		if(0 != playerIsland)
		{
			output = DataObject(tileIsland == playerIsland);
			return;
		}

		//total up the number of islands selected by players

		WX_DECLARE_HASH_MAP(wxInt32, wxInt32, wxIntegerHash, wxIntegerEqual, 
			IntHash);
		IntHash hash;

		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			wxInt32 island = playerGameData<wxInt32>(shHomeIsland, i);

			if(0 != island)
			{
				hash[island]++;
			}
		}
		
		//if the player has not yet selected a home island, then this
		//location will be allowed, as long as half of the players or
		//less (round up) are not on the island
		float dif = numPlayers() - hash[tileIsland];
		float half = float(numPlayers()) * 0.5f;

		output = DataObject((dif > half));
	}
};

IMPLEMENT_LOGIC_MIXIN(LogicPlaceInitialTileHomeIsland, LogicPlaceInitialTile, 
					  TRADE_ROUTES);
