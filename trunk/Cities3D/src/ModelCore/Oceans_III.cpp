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
class Oceans_III : public OceansBase
{
public:
	Oceans_III()
	{
		mName = swOceansIII;
		mInternalName = swOceansIIIInternal;

		data<wxInt32>(shID) = MapOceans_III;
		data<wxInt32>(shPorts) = 8;
		data<wxInt32>(shMinPlayers) = 3;
		data<wxInt32>(shMaxPlayers) = 3;
		data<wxInt32>(shHiddenTiles) = 14;
		data<wxInt32>(shHiddenChips) = 6;

		data<Data::IntHash>(shRandomPorts)[shPort3] = 3;
		data<Data::IntHash>(shHiddenResources)[shTimber] = 1;
		data<Data::IntHash>(shHiddenResources)[shWheat] = 1;
		data<Data::IntHash>(shHiddenResources)[shOre] = 2;
		data<Data::IntHash>(shHiddenResources)[shClay] = 1;
		data<Data::IntHash>(shHiddenResources)[shSheep] = 1;
		data<Data::IntHash>(shHiddenResources)[shOcean] = 8;
		data<Data::IntHash>(shHiddenChipArray)[sh3] = 1;
		data<Data::IntHash>(shHiddenChipArray)[sh5] = 1;
		data<Data::IntHash>(shHiddenChipArray)[sh8] = 1;
		data<Data::IntHash>(shHiddenChipArray)[sh9] = 1;
		data<Data::IntHash>(shHiddenChipArray)[sh11] = 1;
		data<Data::IntHash>(shHiddenChipArray)[sh12] = 1;

		Map::CreateMap(*this, 6, 9, boost::bind(&Oceans_III::SetTileData, 
			this, _1));
	}

private:
	void SetTileData(const wxInt32 index)
	{
		//set the tile type
		switch(index)
		{
		case 7: case 8: case 9: case 10: case 11: case 15: case 16: case 17:
		case 23: case 24: case 30: case 46:
			//set the initial placement to TRUE for all land pieces (except the
			//gold)
			if(46 == index)
			{
				tile<wxInt32>(shInitial, index) = FALSE;
			}
			else
			{
				tile<wxInt32>(shInitial, index) = TRUE;
			}

			break;
		//ports
		case 1: case 3: case 5: case 13: case 18: case 21: case 29: case 37:
			//set port
			tile<HashString>(shTileType, index) = shPort;

			//set the initial placement to FALSE for all other pieces
			tile<wxInt32>(shInitial, index) = FALSE;

			//special port sides
			if(3 == index) tile<wxInt32>(shPortSide, index) = 2;
			if(13 == index) tile<wxInt32>(shPortSide, index) = 5;
			if(18 == index) tile<wxInt32>(shPortSide, index) = 0;
			if(21 == index) tile<wxInt32>(shPortSide, index) = 5;
			if(29 == index) tile<wxInt32>(shPortSide, index) = 4;
			if(37 == index) tile<wxInt32>(shPortSide, index) = 0;
			
			break;
		//random tiles
		case 20: case 27: case 28: case 33: case 34: case 35: case 40: case 41:
		case 42: case 43: case 47: case 48: case 49: case 50:
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
		case 7:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 3;
			break;
		case 8:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 9:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		case 10:
			tile<HashString>(shTileType, index) = shClay;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 11:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 9;
			break;
		case 15:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 5;
			break;
		case 16:
			tile<HashString>(shTileType, index) = shSheep;
			tile<wxInt32>(shRoll, index) = 6;
			break;
		case 17:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 2;
			break;
		case 23:
			tile<HashString>(shTileType, index) = shOre;
			tile<wxInt32>(shRoll, index) = 10;
			break;
		case 24:
			tile<HashString>(shTileType, index) = shTimber;
			tile<wxInt32>(shRoll, index) = 8;
			break;
		case 30:
			tile<HashString>(shTileType, index) = shWheat;
			tile<wxInt32>(shRoll, index) = 11;
			break;
		case 46:
			tile<HashString>(shTileType, index) = shGold;
			tile<wxInt32>(shRoll, index) = 4;
			break;
		default:
			break;
		}
	}
};

IMPLEMENT_OFFICIAL_MAP(Oceans_III);

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



