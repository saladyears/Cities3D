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
#include "IntoTheDesertBase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
class IntoTheDesert_III : public IntoTheDesertBase
{
public:
	IntoTheDesert_III()
	{
		mName = swIntoTheDesertIII;
		mInternalName = swIntoTheDesertIIIInternal;

		data<wxInt32>(shID) = MapIntoTheDesert_III;
		data<wxInt32>(shPorts) = 9;
		data<wxInt32>(shMinPlayers) = 3;
		data<wxInt32>(shMaxPlayers) = 3;
		
		data<Data::IntHash>(shRandomPorts)[shPort3] = 4;

		Map::CreateMap(*this, 7, 9, boost::bind(&IntoTheDesert_III::SetTileData, 
			this, _1));
	}

private:
	void SetTileData(const wxInt32 index)
	{
		//set default values so that all data pieces are initialized
		tile<wxInt32>(shSecondary, index) = FALSE;
		tile<wxInt32>(shRoll, index) = -1;
		tile<wxInt32>(shInitial, index) = FALSE;

		//set the tile type
		switch(index)
		{
		//continent
		case 8: case 16: case 17: case 23: case 24: case 25: case 26: case 31: 
		case 32: case 33: case 34: case 39: case 40: case 41:
			tile<wxInt32>(shInitial, index) = TRUE;
			break;
		//islands
		case 11: case 12: case 13: case 43: case 50: case 53: case 54: case 56: 
		case 58:
			//allow secondary placement for all islands
			tile<wxInt32>(shSecondary, index) = TRUE;
			break;

		//ports
		case 1: case 10: case 15: case 28: case 30: case 38: case 42: case 47: 
		case 48:
			//set port
			tile<HashString>(shTileType, index) = shPort;

			//special port sides
			if(1 == index) tile<wxInt32>(shPortSide, index) = 2;
			if(15 == index) tile<wxInt32>(shPortSide, index) = 3;
			if(30 == index) tile<wxInt32>(shPortSide, index) = 5;
			if(47 == index) tile<wxInt32>(shPortSide, index) = 0;
			if(48 == index) tile<wxInt32>(shPortSide, index) = 5;
			
			break;
		default:
			break;
		}

		//tile pieces
		switch(index)
		{
		case 8:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 9:
			tile<HashString>(shTileType, index) = shDesert;
			break;
		case 11:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 12:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 13:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 16:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 17:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 12;
			break;
		case 18:
			tile<HashString>(shTileType, index) = shDesert;
			break;
		case 19:
			tile<HashString>(shTileType, index) = shDesert;
			break;
		case 20:
			tile<HashString>(shTileType, index) = shDesert;
			break;
		case 23:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 24:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 2;
			break;
		case 25:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 26:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 31:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 32:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 33:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 34:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 39:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		case 40:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		case 41:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 43:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 50:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 53:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 54:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		case 56:
			tile<HashString>(shTileType, index) = shGold;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 58:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		default:
			break;
		}
	}
};

IMPLEMENT_OFFICIAL_MAP(IntoTheDesert_III);

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



