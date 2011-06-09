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
#include "IslandsBase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
class SixIslands_VI : public IslandsBase
{
public:
	SixIslands_VI()
	{
		mName = swSixIslands;
		mInternalName = swSixIslandsInternal;

		data<wxInt32>(shID) = MapSixIslands_VI;
		data<wxInt32>(shPorts) = 11;
		data<wxInt32>(shMinPlayers) = 6;
		data<wxInt32>(shMaxPlayers) = 6;

		data<Data::IntHash>(shRandomPorts)[shPort3] = 6;

		Map::CreateMap(*this, 10, 9, boost::bind(&SixIslands_VI::SetTileData, 
			this, _1));
	}

private:
	void SetTileData(const wxInt32 index)
	{
		//set default values so that all data pieces are initialized
		tile<wxInt32>(shSecondary, index) = FALSE;
		tile<wxInt32>(shRoll, index) = -1;

		//set the tile type
		switch(index)
		{
		case 11: case 12: case 13: case 15: case 18: case 19: case 22: case 23:
		case 25: case 26: case 28: case 29: case 32: case 33: case 36: case 50:
		case 54: case 57: case 58: case 60: case 61: case 64: case 65: case 67:
		case 68: case 70: case 71: case 74: case 75: case 78: case 81: case 82:
			//set the initial placement to TRUE for all land pieces
			tile<wxInt32>(shInitial, index) = TRUE;

			//allow secondary placement for all islands
			tile<wxInt32>(shSecondary, index) = TRUE;

			break;
		//ports
		case 1: case 5: case 8: case 34: case 38: case 43: case 63: case 72:
		case 76: case 88: case 91:
			//set port
			tile<HashString>(shTileType, index) = shPort;

			//set the initial placement to FALSE for all other pieces
			tile<wxInt32>(shInitial, index) = FALSE;

			//special port sides
			if(8 == index) tile<wxInt32>(shPortSide, index) = 3;
			if(72 == index) tile<wxInt32>(shPortSide, index) = 1;
			
			break;
		default:
			//set the initial placement to FALSE for all other pieces
			tile<wxInt32>(shInitial, index) = FALSE;

			break;
		}

		//tile pieces
		switch(index)
		{
		case 11:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 2;
			break;
		case 12:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 13:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		case 15:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 18:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 19:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 22:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 23:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 25:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		case 26:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 28:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		case 29:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 32:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		case 33:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 36:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 50:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 54:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 57:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 58:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 60:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 61:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 64:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 65:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 12;
			break;
		case 67:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 68:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		case 70:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 71:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		case 74:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 75:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 78:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 2;
			break;
		case 81:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 82:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 12;
			break;
		default:
			break;
		}
	}
};

IMPLEMENT_OFFICIAL_MAP(SixIslands_VI);

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



