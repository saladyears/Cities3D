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
#include "GreatCrossingBase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
class GreatCrossing_IV : public GreatCrossingBase
{
public:
	GreatCrossing_IV()
	{
		mName = swTheGreatCrossingIV;
		mInternalName = swTheGreatCrossingIVInternal;

		data<wxInt32>(shID) = MapGreatCrossing_IV;
		data<wxInt32>(shPorts) = 11;
		data<wxInt32>(shMinPlayers) = 4;
		data<wxInt32>(shMaxPlayers) = 4;

		data<Data::IntHash>(shRandomPorts)[shPort3] = 6;

		Map::CreateMap(*this, 7, 9, boost::bind(&GreatCrossing_IV::SetTileData, 
			this, _1));
	}

private:
	void SetTileData(const wxInt32 index)
	{
		//set default values so that all data pieces are initialized
		tile<wxInt32>(shIsland, index) = -1;
		tile<wxInt32>(shInitial, index) = FALSE;

		//set the tile type
		switch(index)
		{
		//left island
		case 8: case 9: case 16: case 17: case 23: case 24: case 31: case 38: 
		case 39: case 46: case 53:
			//set the initial placement to TRUE for all land pieces
			tile<wxInt32>(shInitial, index) = TRUE;

			//this is a different island
			tile<wxInt32>(shIsland, index) = 1;

			break;
		//right island
		case 13: case 20: case 27: case 28: case 35: case 42: case 43: case 49: 
		case 50: case 57: case 58:
			//set the initial placement to TRUE for all land pieces
			tile<wxInt32>(shInitial, index) = TRUE;

			//this is a different island
			tile<wxInt32>(shIsland, index) = 2;

			break;
		//ports
		case 0: case 10: case 15: case 19: case 21: case 30: case 36: case 45: 
		case 47: case 56: case 66:
			//set port
			tile<HashString>(shTileType, index) = shPort;

			//special port sides
			if(15 == index) tile<wxInt32>(shPortSide, index) = 4;
			if(21 == index) tile<wxInt32>(shPortSide, index) = 0;
			if(30 == index) tile<wxInt32>(shPortSide, index) = 4;
			if(45 == index) tile<wxInt32>(shPortSide, index) = 4;

			break;
		default:
			break;
		}

		//tile pieces
		switch(index)
		{
		case 8:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 2;
			break;
		case 9:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 13:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 12;
			break;
		case 16:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 17:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 20:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 23:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 24:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 27:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 2;
			break;
		case 28:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 31:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 35:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 38:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		case 39:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		case 42:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		case 43:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 46:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 49:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 50:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 53:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 12;
			break;
		case 57:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		case 58:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		default:
			break;
		}
	}
};

IMPLEMENT_OFFICIAL_MAP(GreatCrossing_IV);

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



