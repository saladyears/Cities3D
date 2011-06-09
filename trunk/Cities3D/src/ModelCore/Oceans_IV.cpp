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
class Oceans_IV : public OceansBase
{
public:
	Oceans_IV()
	{
		mName = swOceansIV;
		mInternalName = swOceansIVInternal;

		data<wxInt32>(shID) = MapOceans_IV;
		data<wxInt32>(shPorts) = 9;
		data<wxInt32>(shMinPlayers) = 4;
		data<wxInt32>(shMaxPlayers) = 4;
		data<wxInt32>(shHiddenTiles) = 22;
		data<wxInt32>(shHiddenChips) = 7;

		data<Data::IntHash>(shRandomPorts)[shPort3] = 4;
		data<Data::IntHash>(shHiddenResources)[shTimber] = 1;
		data<Data::IntHash>(shHiddenResources)[shWheat] = 2;
		data<Data::IntHash>(shHiddenResources)[shOre] = 1;
		data<Data::IntHash>(shHiddenResources)[shClay] = 1;
		data<Data::IntHash>(shHiddenResources)[shSheep] = 2;
		data<Data::IntHash>(shHiddenResources)[shDesert] = 1;
		data<Data::IntHash>(shHiddenResources)[shOcean] = 14;
		data<Data::IntHash>(shHiddenChipArray)[sh3] = 1;
		data<Data::IntHash>(shHiddenChipArray)[sh4] = 1;
		data<Data::IntHash>(shHiddenChipArray)[sh5] = 2;
		data<Data::IntHash>(shHiddenChipArray)[sh6] = 1;
		data<Data::IntHash>(shHiddenChipArray)[sh9] = 1;
		data<Data::IntHash>(shHiddenChipArray)[sh10] = 1;

		Map::CreateMap(*this, 8, 9, boost::bind(&Oceans_IV::SetTileData, 
			this, _1));
	}

private:
	void SetTileData(const wxInt32 index)
	{
		//set the tile type
		switch(index)
		{
		case 9: case 10: case 11: case 12: case 13: case 14: case 15: case 18:
		case 19: case 20: case 21: case 22: case 23: case 26: case 32: case 60:
		case 66:
			//set the initial placement to TRUE for all land pieces (except the
			//gold)
			if(60 == index || 66 == index)
			{
				tile<wxInt32>(shInitial, index) = FALSE;
			}
			else
			{
				tile<wxInt32>(shInitial, index) = TRUE;
			}

			break;
		//ports
		case 1: case 4: case 6: case 17: case 24: case 27: case 30: case 34:
		case 41:
			//set port
			tile<HashString>(shTileType, index) = shPort;

			//set the initial placement to FALSE for all other pieces
			tile<wxInt32>(shInitial, index) = FALSE;

			//special port sides
			if(4 == index) tile<wxInt32>(shPortSide, index) = 2;
			if(6 == index) tile<wxInt32>(shPortSide, index) = 3;
			if(27 == index) tile<wxInt32>(shPortSide, index) = 5;
			if(30 == index) tile<wxInt32>(shPortSide, index) = 0;
			if(34 == index) tile<wxInt32>(shPortSide, index) = 5;
			if(41 == index) tile<wxInt32>(shPortSide, index) = 0;
			
			break;
		//random tiles
		case 36: case 37: case 38: case 39: case 43: case 44: case 45: case 46:
		case 47: case 48: case 49: case 52: case 53: case 54: case 55: case 56:
		case 57: case 61: case 62: case 63: case 64: case 65:
			//set piece type
			tile<HashString>(shTileType, index) = shHidden;

			//initial placement is false
			tile<wxInt32>(shInitial, index) = FALSE;

			break;
		//standard tiles
		default:
			//set the initial placement to FALSE for all other pieces
			tile<wxInt32>(shInitial, index) = FALSE;

			//set type
			tile<HashString>(shTileType, index) = shOcean;

			break;
		}

		//land tiles
		switch(index)
		{
		case 9:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 10:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		case 11:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 12:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		case 13:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 12;
			break;
		case 14:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 15:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		case 18:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 19:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 20:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 21:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 22:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 23:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 26:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 2;
			break;
		case 32:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 60:
			tile<HashString>(shTileType, index) = shGold;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 66:
			tile<HashString>(shTileType, index) = shGold;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		default:
			break;
		}
	}
};

IMPLEMENT_OFFICIAL_MAP(Oceans_IV);

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



