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
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicIsResourceBankTradeableStandard
// 
// Determines whether the player can trade this resource with the bank in a 
// standard game.
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
// Mixin To:
//     <LogicIsResourceBankTradeable>
//
class LogicIsResourceBankTradeableStandard : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		wxASSERT(2 <= input.numItems());

		bool canTrade = false;
		wxInt32 ratio = -1;

		const HashString& resource = input.read<HashString>();
		wxInt32 amount = input.read<wxInt32>(1);

		if(2 <= amount)
		{
			bool has3to1 = false;
			bool has2to1 = false;
			typedef std::set<HashString> StringSet;
			StringSet tileTypes;

			// In a standard game, a player can always trade the resource to 
			// the bank 4-1.  If they have any 3-1 port, they can trade 3-1, 
			// and if they have a 2-1 port of that resource type, they can 
			// trade 2-1.

			// Check for settlement ports.
			const PlayerGame::CornerObjectArray &settlements = 
				playerGame().getCornerObjects(shSettlements);
			PlayerGame::CornerObjectArray::const_iterator it, 
				itEnd = settlements.end();
			for(it = settlements.begin(); it != itEnd; ++it)
			{
				// The settlement has to actually be on one of the two port
				// indices of the tile to get counted.
				CornerObject::TileCorner tileCorner = (*it)->tile1();
				if(true == CheckForPort(tileCorner))
				{
					tileTypes.insert(tile<HashString>(shTileType, 
						tileCorner.first));
				}

				tileCorner = (*it)->tile2();
				if(true == CheckForPort(tileCorner))
				{
					tileTypes.insert(tile<HashString>(shTileType, 
						tileCorner.first));
				}
				
				tileCorner = (*it)->tile3();
				if(true == CheckForPort(tileCorner))
				{
					tileTypes.insert(tile<HashString>(shTileType, 
						tileCorner.first));
				}
			}

			// Repeat for cities.
			const PlayerGame::CornerObjectArray &cities = 
				playerGame().getCornerObjects(shCities);
			itEnd = cities.end();
			for(it = cities.begin(); it != itEnd; ++it)
			{
				// The city has to actually be on one of the two port
				// indices of the tile to get counted.
				CornerObject::TileCorner tileCorner = (*it)->tile1();
				if(true == CheckForPort(tileCorner))
				{
					tileTypes.insert(tile<HashString>(shTileType, 
						tileCorner.first));
				}

				tileCorner = (*it)->tile2();
				if(true == CheckForPort(tileCorner))
				{
					tileTypes.insert(tile<HashString>(shTileType, 
						tileCorner.first));
				}
				
				tileCorner = (*it)->tile3();
				if(true == CheckForPort(tileCorner))
				{
					tileTypes.insert(tile<HashString>(shTileType, 
						tileCorner.first));
				}
			}

			// Now that we have a set of all tile types the player is on, check
			// for a 3 to 1, and then for a 2 for 1 of this resource type.
			if(tileTypes.end() != tileTypes.find(shPort3))
			{				
				has3to1 = true;
			}

			HashString port = HashString::Format(shPortString.c_str(), 
                resource.c_str());
			if(tileTypes.end() != tileTypes.find(port))
			{
				has2to1 = true;
			}

			// Finally, see if they can trade it.
			if( (2 <= amount) &&
				(true == has2to1))
			{
				canTrade = true;
				ratio = 2;
			}
			else if((3 <= amount) &&
					(true == has3to1))
			{
				canTrade = true;
				ratio = 3;
			}
			else if(4 <= amount)
			{
				canTrade = true;
				ratio = 4;
			}
		}
		
		output = DataObject(canTrade, ratio);
	}

private:
	bool CheckForPort(CornerObject::TileCorner tileCorner)
	{
		wxInt32 thisTile = tileCorner.first;
		wxInt32 corner = tileCorner.second;
		wxInt32 port1 = tile<wxInt32>(shPort1, thisTile);
		wxInt32 port2 = tile<wxInt32>(shPort2, thisTile);

		return ( (corner == port1) || (corner == port2) );
	}
};

IMPLEMENT_LOGIC_MIXIN(LogicIsResourceBankTradeableStandard, 
					  LogicIsResourceBankTradeable, STANDARD)
