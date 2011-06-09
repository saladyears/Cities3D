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
class FourIslands_III : public IslandsBase
{
public:
	FourIslands_III()
	{
		mName = swFourIslandsIII;
		mInternalName = swFourIslandsIIIInternal;

		data<wxInt32>(shID) = MapFourIslands_III;
		data<wxInt32>(shPorts) = 8;
		data<wxInt32>(shMinPlayers) = 3;
		data<wxInt32>(shMaxPlayers) = 3;

		data<Data::IntHash>(shRandomPorts)[shPort3] = 3;

		Map::CreateMap(*this, 6, 9, boost::bind(&FourIslands_III::SetTileData, 
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
		case 7: case 8: case 11: case 14: case 17: case 23: case 24: case 33:
		case 34: case 40: case 41: case 43: case 46: case 47: case 50:
			//set the initial placement to TRUE for all land pieces
			tile<wxInt32>(shInitial, index) = TRUE;

			//allow secondary placement for all islands
			tile<wxInt32>(shSecondary, index) = TRUE;

			break;
		//ports
		case 1: case 5: case 18: case 28: case 30: case 48: case 53: case 57:
			//set port
			tile<HashString>(shTileType, index) = shPort;

			//set the initial placement to FALSE for all other pieces
			tile<wxInt32>(shInitial, index) = FALSE;

			//special port sides
			if(1 == index) tile<wxInt32>(shPortSide, index) = 3;
			if(18 == index) tile<wxInt32>(shPortSide, index) = 2;
			if(53 == index) tile<wxInt32>(shPortSide, index) = 0;
			
			break;
		default:
			//set the initial placement to FALSE for all other pieces
			tile<wxInt32>(shInitial, index) = FALSE;

			break;
		}

		//tile pieces
		switch(index)
		{
		case 7:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 8:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 11:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		case 14:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 17:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 23:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 24:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 2;
			break;
		case 33:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 34:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		case 40:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 41:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 43:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 46:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 47:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 12;
			break;
		case 50:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		default:
			break;
		}
	}
};

IMPLEMENT_OFFICIAL_MAP(FourIslands_III);

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



