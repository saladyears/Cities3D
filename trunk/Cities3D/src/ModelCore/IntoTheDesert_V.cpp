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
class IntoTheDesert_V : public IntoTheDesertBase
{
public:
	IntoTheDesert_V()
	{
		mName = swIntoTheDesertV;
		mInternalName = swIntoTheDesertVInternal;

		data<wxInt32>(shID) = MapIntoTheDesert_V;
		data<wxInt32>(shPorts) = 10;
		data<wxInt32>(shMinPlayers) = 5;
		data<wxInt32>(shMaxPlayers) = 5;
		data<wxInt32>(shAllowedCards) = 9;

		data<Data::IntHash>(shRandomPorts)[shPort3] = 5;

		Map::CreateMap(*this, 10, 9, boost::bind(&IntoTheDesert_V::SetTileData, 
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
		case 35: case 36: case 37: case 38: case 43: case 44: case 45: case 46: 
		case 47: case 48: case 53: case 54: case 55: case 56: case 57: case 58:
		case 64: case 74:
			tile<wxInt32>(shInitial, index) = TRUE;
			break;
		//islands
		case 11: case 12: case 13: case 14: case 16: case 17: case 18: case 19: 
		case 50: case 71: case 76: case 77: case 79: case 81: case 82:
			//allow secondary placement for all islands
			tile<wxInt32>(shSecondary, index) = TRUE;
			break;

		//ports
		case 33: case 34: case 39: case 42: case 59: case 63: case 65: case 67: 
		case 68: case 75:
			//set port
			tile<HashString>(shTileType, index) = shPort;

			//special port sides
			if(34 == index) tile<wxInt32>(shPortSide, index) = 3;
			if(39 == index) tile<wxInt32>(shPortSide, index) = 0;
			if(59 == index) tile<wxInt32>(shPortSide, index) = 0;
			if(63 == index) tile<wxInt32>(shPortSide, index) = 4;
			if(67 == index) tile<wxInt32>(shPortSide, index) = 0;
			if(68 == index) tile<wxInt32>(shPortSide, index) = 5;

			break;
		default:
			break;
		}

		//tile pieces
		switch(index)
		{
		case 11:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 12;
			break;
		case 12:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 13:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 14:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		case 16:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 17:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 18:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 19:
			tile<HashString>(shTileType, index) = shGold;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		case 24:
			tile<HashString>(shTileType, index) = shDesert;
			break;
		case 25:
			tile<HashString>(shTileType, index) = shDesert;
			break;
		case 26:
			tile<HashString>(shTileType, index) = shDesert;
			break;
		case 27:
			tile<HashString>(shTileType, index) = shDesert;
			break;
		case 28:
			tile<HashString>(shTileType, index) = shDesert;
			break;
		case 35:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 2;
			break;
		case 36:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 37:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 38:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 43:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 44:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 12;
			break;
		case 45:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 46:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 47:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 48:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 50:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 53:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 54:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		case 55:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 56:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		case 57:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 58:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 64:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 71:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 74:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 76:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 77:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		case 79:
			tile<HashString>(shTileType, index) = shGold;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 81:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		case 82:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 2;
			break;
		default:
			break;
		}
	}
};

IMPLEMENT_OFFICIAL_MAP(IntoTheDesert_V);

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



