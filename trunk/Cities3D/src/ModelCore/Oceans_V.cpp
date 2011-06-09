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
#include "OceansBase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
class Oceans_V : public OceansBase
{
public:
	Oceans_V()
	{
		mName = swOceansV;
		mInternalName = swOceansVInternal;

		data<wxInt32>(shID) = MapOceans_V;
		data<wxInt32>(shPorts) = 9;
		data<wxInt32>(shMinPlayers) = 5;
		data<wxInt32>(shMaxPlayers) = 5;
		data<wxInt32>(shHiddenTiles) = 26;
		data<wxInt32>(shHiddenChips) = 11;
		data<wxInt32>(shAllowedCards) = 9;

		data<Data::IntHash>(shRandomPorts)[shPort3] = 4;
		data<Data::IntHash>(shHiddenResources)[shTimber] = 3;
		data<Data::IntHash>(shHiddenResources)[shWheat] = 2;
		data<Data::IntHash>(shHiddenResources)[shOre] = 2;
		data<Data::IntHash>(shHiddenResources)[shClay] = 2;
		data<Data::IntHash>(shHiddenResources)[shSheep] = 2;
		data<Data::IntHash>(shHiddenResources)[shOcean] = 15;
		data<Data::IntHash>(shHiddenChipArray)[sh3] = 1;
		data<Data::IntHash>(shHiddenChipArray)[sh4] = 1;
		data<Data::IntHash>(shHiddenChipArray)[sh5] = 2;
		data<Data::IntHash>(shHiddenChipArray)[sh8] = 1;
		data<Data::IntHash>(shHiddenChipArray)[sh9] = 2;
		data<Data::IntHash>(shHiddenChipArray)[sh10] = 2;
		data<Data::IntHash>(shHiddenChipArray)[sh11] = 2;

		Map::CreateMap(*this, 10, 9, boost::bind(&Oceans_V::SetTileData, 
			this, _1));
	}

private:
	void SetTileData(const wxInt32 index)
	{
		tile<wxInt32>(shInitial, index) = FALSE;

		//set the tile type
		switch(index)
		{
		case 13: case 14: case 15: case 16: case 17: case 24: case 25:
		case 26: case 27: case 35: case 36: case 37: case 46: case 47: case 48:
		case 57: case 58: case 68:
			tile<wxInt32>(shInitial, index) = TRUE;
			break;
		//ports
		case 3: case 5: case 7: case 12: case 34: case 38: case 56: case 69:
		case 79:
			//set port
			tile<HashString>(shTileType, index) = shPort;

			//special port sides
			if(5 == index) tile<wxInt32>(shPortSide, index) = 3;
			if(34 == index) tile<wxInt32>(shPortSide, index) = 5;
			
			break;
		//random tiles
		case 11: case 19: case 22: case 29: case 32: case 33: case 39: case 40:
		case 43: case 44: case 50: case 53: case 54: case 55: case 60: case 61:
		case 64: case 65: case 66: case 70: case 71: case 75: case 76: case 77:
		case 80: case 81:
			//set piece type
			tile<HashString>(shTileType, index) = shHidden;

			break;
		//standard tiles
		default:
			//set type
			tile<HashString>(shTileType, index) = shOcean;

			break;
		}

		//land tiles
		switch(index)
		{
		case 13:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 14:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		case 15:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 16:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		case 17:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 12;
			break;
		case 24:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 25:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 26:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 27:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 35:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 2;
			break;
		case 36:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 37:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		case 46:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 47:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 48:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 57:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 58:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 68:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 74:
			tile<HashString>(shTileType, index) = shGold;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 82:
			tile<HashString>(shTileType, index) = shGold;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		default:
			break;
		}
	}
};

IMPLEMENT_OFFICIAL_MAP(Oceans_V);

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



