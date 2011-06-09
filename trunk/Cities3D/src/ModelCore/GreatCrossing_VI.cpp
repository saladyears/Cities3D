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
class GreatCrossing_VI : public GreatCrossingBase
{
public:
	GreatCrossing_VI()
	{
		mName = swTheGreatCrossingVI;
		mInternalName = swTheGreatCrossingVIInternal;

		data<wxInt32>(shID) = MapGreatCrossing_VI;
		data<wxInt32>(shPorts) = 12;
		data<wxInt32>(shMinPlayers) = 6;
		data<wxInt32>(shMaxPlayers) = 6;
		data<wxInt32>(shAllowedCards) = 9;

		data<Data::IntHash>(shRandomPorts)[shPort3] = 7;

		Map::CreateMap(*this, 9, 9, boost::bind(&GreatCrossing_VI::SetTileData, 
			this, _1));
	}

private:
	void SetTileData(const wxInt32 index)
	{
		//set default values so that all data pieces are initialized
		tile<wxInt32>(shIsland, index) = -1;
					
		//set the initial placement to FALSE for all other pieces
		tile<wxInt32>(shInitial, index) = FALSE;

		//set the tile type
		switch(index)
		{
		//left island
		case 10: case 11: case 12: case 13: case 14: case 15: case 16: case 17: 
		case 20: case 21: case 22: case 23: case 24: case 25: case 26:
			//set land type
			//set the initial placement to TRUE for all land pieces
			tile<wxInt32>(shInitial, index) = TRUE;

			//this is a different island
			tile<wxInt32>(shIsland, index) = 1;

			break;
		//right island
		case 58: case 59: case 60: case 61: case 62: case 63: case 64: case 67: 
		case 68: case 69: case 70: case 71: case 72: case 73: case 74:
			//set the initial placement to TRUE for all land pieces
			tile<wxInt32>(shInitial, index) = TRUE;

			//this is a different island
			tile<wxInt32>(shIsland, index) = 2;

			break;
		//ports
		case 2: case 4: case 7: case 27: case 30: case 34: case 49: case 53: 
		case 55: case 78: case 81: case 83:
			//set port
			tile<HashString>(shTileType, index) = shPort;

			//special port sides
			if(4 == index) tile<wxInt32>(shPortSide, index) = 3;
			if(27 == index) tile<wxInt32>(shPortSide, index) = 0;
			if(30 == index) tile<wxInt32>(shPortSide, index) = 5;
			if(34 == index) tile<wxInt32>(shPortSide, index) = 5;
			if(49 == index) tile<wxInt32>(shPortSide, index) = 3;
			if(53 == index) tile<wxInt32>(shPortSide, index) = 3;
			if(78 == index) tile<wxInt32>(shPortSide, index) = 5;

			break;
		default:
			//set the initial placement to FALSE for all other pieces
			tile<wxInt32>(shInitial, index) = FALSE;

			break;
		}

		//tile pieces
		switch(index)
		{
		case 10:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 11:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 12:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 13:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 14:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 12;
			break;
		case 15:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 16:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		case 17:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 20:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		case 21:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 22:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 23:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 24:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 2;
			break;
		case 25:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 26:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 58:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 12;
			break;
		case 59:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 60:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 61:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 62:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 63:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 64:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		case 67:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 68:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 69:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 2;
			break;
		case 70:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 71:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 72:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 73:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		case 74:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		default:
			break;
		}
	}
};

IMPLEMENT_OFFICIAL_MAP(GreatCrossing_VI);

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



