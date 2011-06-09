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
class IntoTheDesert_VI : public IntoTheDesertBase
{
public:
	IntoTheDesert_VI()
	{
		mName = swIntoTheDesertVI;
		mInternalName = swIntoTheDesertVIInternal;

		data<wxInt32>(shID) = MapIntoTheDesert_VI;
		data<wxInt32>(shPorts) = 10;
		data<wxInt32>(shMinPlayers) = 6;
		data<wxInt32>(shMaxPlayers) = 6;
		data<wxInt32>(shAllowedCards) = 9;

		data<Data::IntHash>(shRandomPorts)[shPort3] = 5;

		Map::CreateMap(*this, 11, 9, boost::bind(&IntoTheDesert_VI::SetTileData, 
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
		case 39: case 40: case 41: case 42: case 47: case 48: case 49: case 50: 
		case 51: case 52: case 53: case 58: case 59: case 60: case 61: case 62: 
		case 63: case 64: case 70: case 71: case 81: case 82:
			tile<wxInt32>(shInitial, index) = TRUE;
			break;
		//islands
		case 12: case 13: case 14: case 15: case 16: case 18: case 19: case 20: 
		case 21: case 55: case 78: case 84: case 85: case 87: case 89: case 90:
			//allow secondary placement for all islands
			tile<wxInt32>(shSecondary, index) = TRUE;
			break;

		//ports
		case 37: case 38: case 43: case 46: case 65: case 69: case 72: case 74: 
		case 75: case 83:
			//set port
			tile<HashString>(shTileType, index) = shPort;

			//special port sides
			if(38 == index) tile<wxInt32>(shPortSide, index) = 3;
			if(43 == index) tile<wxInt32>(shPortSide, index) = 0;
			if(65 == index) tile<wxInt32>(shPortSide, index) = 0;
			if(74 == index) tile<wxInt32>(shPortSide, index) = 0;
			if(75 == index) tile<wxInt32>(shPortSide, index) = 5;

			break;
		default:
			break;
		}

		//tile pieces
		switch(index)
		{

		case 12:
			tile<HashString>(shTileType, index) = shGold;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 13:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 12;
			break;
		case 14:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 15:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 16:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		case 18:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 19:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 20:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 21:
			tile<HashString>(shTileType, index) = shGold;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		case 27:
			tile<HashString>(shTileType, index) = shDesert;
			break;
		case 28:
			tile<HashString>(shTileType, index) = shDesert;
			break;
		case 29:
			tile<HashString>(shTileType, index) = shDesert;
			break;
		case 30:
			tile<HashString>(shTileType, index) = shDesert;
			break;
		case 31:
			tile<HashString>(shTileType, index) = shDesert;
			break;
		case 39:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 2;
			break;
		case 40:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 41:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 42:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 47:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 2;
			break;
		case 48:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 49:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 12;
			break;
		case 50:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 51:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 52:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 53:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 55:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 58:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 12;
			break;
		case 59:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 60:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		case 61:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 62:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		case 63:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		case 64:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 70:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		case 71:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 78:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 81:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 82:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 84:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 85:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		case 87:
			tile<HashString>(shTileType, index) = shGold;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 89:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		case 90:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 2;
			break;
		default:
			break;
		}
	}
};

IMPLEMENT_OFFICIAL_MAP(IntoTheDesert_VI);

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



