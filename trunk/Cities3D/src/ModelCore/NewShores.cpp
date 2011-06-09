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
#include "OfficialMap.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
class NewShores : public OfficialMap
{
public:
	NewShores()
	{
		mName = swNewShores;
		mInternalName = swNewShoresInternal;

		data<wxInt32>(shID) = MapNewShores;
		data<wxInt32>(shPoints) = 13;
		data<wxInt32>(shRandomTiles) = 19;
		data<wxInt32>(shRandomChips) = 0;
		data<wxInt32>(shPorts) = 9;
		data<wxInt32>(shMinPlayers) = 3;
		data<wxInt32>(shMaxPlayers) = 4;
		data<wxInt32>(shStockShips) = 15;		

		data<IntArray>(shRollPlaces).resize(6);
		data<IntArray>(shRollPlaces)[0] = 27;
		data<IntArray>(shRollPlaces)[1] = 43;
		data<IntArray>(shRollPlaces)[2] = 61;
		data<IntArray>(shRollPlaces)[3] = 63;
		data<IntArray>(shRollPlaces)[4] = 47;
		data<IntArray>(shRollPlaces)[5] = 29;

		data<Data::IntHash>(shRandomResources)[shTimber] = 4;
		data<Data::IntHash>(shRandomResources)[shWheat] = 4;
		data<Data::IntHash>(shRandomResources)[shOre] = 3;
		data<Data::IntHash>(shRandomResources)[shClay] = 3;
		data<Data::IntHash>(shRandomResources)[shSheep] = 4;
		data<Data::IntHash>(shRandomResources)[shDesert] = 1;
		data<Data::IntHash>(shRandomPorts)[shPort3] = 4;
		data<Data::IntHash>(shRandomPorts)[shPortTimber] = 1;
		data<Data::IntHash>(shRandomPorts)[shPortWheat] = 1;
		data<Data::IntHash>(shRandomPorts)[shPortOre] = 1;
		data<Data::IntHash>(shRandomPorts)[shPortClay] = 1;
		data<Data::IntHash>(shRandomPorts)[shPortSheep] = 1;

		mRuleSets.push_back(std::make_pair(swStandardRules, swGold));
		mRuleSets.push_back(std::make_pair(swSeafarersRules, 
			swSeafarers));
		mRuleSets.push_back(std::make_pair(swStandardRules, 
			swRandomTiles));
		mRuleSets.push_back(std::make_pair(swStandardRules, 
			swAroundTheBoard));
		mRuleSets.push_back(std::make_pair(swSeafarersRules, 
			swSingleBonus));

		Map::CreateMap(*this, 8, 9, boost::bind(&NewShores::SetTileData, 
			this, _1));
	}

private:
	void SetTileData(const wxInt32 index)
	{
		//set default values so that all data pieces are initialized
		tile<wxInt32>(shSecondary, index) = FALSE;

		//set tile types
		switch(index)
		{
		case 27: case 28: case 29: case 35: case 36: case 37: case 38: case 43:
		case 44: case 45: case 46: case 47: case 52: case 53: case 54: case 55:
		case 61: case 62: case 63:
			//set land type
			tile<HashString>(shTileType, index) = shRandom;

			//set the initial placement to TRUE for all land pieces
			tile<wxInt32>(shInitial, index) = TRUE;

			break;
		//islands
		case 9: case 10: case 13: case 14: case 15: case 40: case 49: case 66:
			//no initial
			tile<wxInt32>(shInitial, index) = FALSE;

			//secondary
			tile<wxInt32>(shSecondary, index) = TRUE;

			break;
		//ports
		case 18: case 20: case 30: case 34: case 48: case 51: case 64: case 69:
		case 71:
			//set port
			tile<HashString>(shTileType, index) = shPort;

			//set the initial placement to FALSE for all other pieces
			tile<wxInt32>(shInitial, index) = FALSE;

			break;
		default:
			//ocean tiles
			tile<HashString>(shTileType, index) = shOcean;

			//set the initial placement to FALSE for all other pieces
			tile<wxInt32>(shInitial, index) = FALSE;

			break;
		}

		//tile pieces
		switch(index)
		{
		case 9:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		case 10:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 13:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 14:
			tile<HashString>(shTileType, index) = shGold;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		case 15:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 40:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 49:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 2;
			break;
		case 66:
			tile<HashString>(shTileType, index) = shGold;		
			tile<wxInt32>(shRoll, index) = 9;
			break;
		}
	}
};

IMPLEMENT_OFFICIAL_MAP(NewShores);

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



