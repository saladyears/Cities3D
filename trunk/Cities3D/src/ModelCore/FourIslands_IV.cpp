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
class FourIslands_IV : public IslandsBase
{
public:
	FourIslands_IV()
	{
		mName = swFourIslandsIV;
		mInternalName = swFourIslandsIVInternal;

		data<wxInt32>(shID) = MapFourIslands_IV;
		data<wxInt32>(shPorts) = 10;
		data<wxInt32>(shMinPlayers) = 4;
		data<wxInt32>(shMaxPlayers) = 4;

		data<Data::IntHash>(shRandomPorts)[shPort3] = 5;

		Map::CreateMap(*this, 7, 9, boost::bind(&FourIslands_IV::SetTileData, 
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
		case 8: case 9: case 10: case 12: case 13: case 16: case 17: case 20:
		case 23: case 24: case 27: case 42: case 43: case 46: case 47: case 49:
		case 50: case 53: case 54: case 57: case 58:
			//set the initial placement to TRUE for all land pieces
			tile<wxInt32>(shInitial, index) = TRUE;

			//allow secondary placement for all islands
			tile<wxInt32>(shSecondary, index) = TRUE;

			break;
		//ports
		case 0: case 2: case 5: case 15: case 19: case 25: case 51: case 61:
		case 64: case 66:
			//set port
			tile<HashString>(shTileType, index) = shPort;

			//set the initial placement to FALSE for all other pieces
			tile<wxInt32>(shInitial, index) = FALSE;

			//special port sides
			if(2 == index) tile<wxInt32>(shPortSide, index) = 3;
			if(5 == index) tile<wxInt32>(shPortSide, index) = 3;
			if(19 == index) tile<wxInt32>(shPortSide, index) = 5;
			
			break;
		default:
			//set the initial placement to FALSE for all other pieces
			tile<wxInt32>(shInitial, index) = FALSE;

			break;
		}

		//tile pieces
		switch(index)
		{
		case 8:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		case 9:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 10:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		case 12:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 13:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 16:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 17:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 20:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 23:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 12;
			break;
		case 24:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 27:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		case 42:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 43:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 46:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 47:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 49:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 50:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		case 53:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 54:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 57:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 2;
			break;
		case 58:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		default:
			break;
		}
	}
};

IMPLEMENT_OFFICIAL_MAP(FourIslands_IV);

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



