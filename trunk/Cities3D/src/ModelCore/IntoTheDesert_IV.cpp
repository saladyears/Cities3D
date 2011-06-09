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
class IntoTheDesert_IV : public IntoTheDesertBase
{
public:
	IntoTheDesert_IV()
	{
		mName = swIntoTheDesertIV;
		mInternalName = swIntoTheDesertIVInternal;

		data<wxInt32>(shID) = MapIntoTheDesert_IV;
		data<wxInt32>(shPorts) = 9;
		data<wxInt32>(shMinPlayers) = 4;
		data<wxInt32>(shMaxPlayers) = 4;

		data<Data::IntHash>(shRandomPorts)[shPort3] = 4;

		Map::CreateMap(*this, 8, 9, boost::bind(&IntoTheDesert_IV::SetTileData, 
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
		case 9: case 18: case 19: case 26: case 27: case 28: case 29: case 30: 
		case 35: case 36: case 37: case 38: case 44: case 45: case 46:
			tile<wxInt32>(shInitial, index) = TRUE;
			break;
		//islands
		case 12: case 13: case 14: case 15: case 49: case 57: case 60: case 61: 
		case 63: case 65: case 66:
			tile<wxInt32>(shSecondary, index) = TRUE;
			break;

		//ports
		case 1: case 11: case 17: case 31: case 34: case 43: case 47: case 53: 
		case 54:
			tile<HashString>(shTileType, index) = shPort;

			//special port sides
			if(1 == index) tile<wxInt32>(shPortSide, index) = 2;
			if(11 == index) tile<wxInt32>(shPortSide, index) = 2;
			if(17 == index) tile<wxInt32>(shPortSide, index) = 3;
			if(31 == index) tile<wxInt32>(shPortSide, index) = 0;
			if(34 == index) tile<wxInt32>(shPortSide, index) = 5;
			if(53 == index) tile<wxInt32>(shPortSide, index) = 0;
			if(54 == index) tile<wxInt32>(shPortSide, index) = 5;

			break;
		default:
			break;
		}

		//tile pieces
		switch(index)
		{
		case 9:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 10:
			tile<HashString>(shTileType, index) = shDesert;
			break;
		case 12:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 13:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 14:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 15:
			tile<HashString>(shTileType, index) = shGold;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		case 18:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 19:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 12;
			break;
		case 20:
			tile<HashString>(shTileType, index) = shDesert;
			break;
		case 21:
			tile<HashString>(shTileType, index) = shDesert;
			break;
		case 22:
			tile<HashString>(shTileType, index) = shDesert;
			break;
		case 26:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 27:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 2;
			break;
		case 28:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 29:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 30:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 35:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 36:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 37:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 38:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 44:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		case 45:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		case 46:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 49:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 57:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 60:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 61:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		case 63:
			tile<HashString>(shTileType, index) = shGold;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 65:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		case 66:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 2;
			break;
		default:
			break;
		}
	}
};

IMPLEMENT_OFFICIAL_MAP(IntoTheDesert_IV);

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



