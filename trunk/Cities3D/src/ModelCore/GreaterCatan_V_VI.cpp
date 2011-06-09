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
#include "GreaterCatanBase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
class GreaterCatan_V_VI : public GreaterCatanBase
{
public:
	GreaterCatan_V_VI()
	{
		mName = swTheBigIslandVVI;
		mInternalName = swTheBigIslandVVIInternal;

		data<wxInt32>(shID) = MapGreaterCatan_V_VI;
		data<wxInt32>(shRandomTiles) = 30;
		data<wxInt32>(shRandomChips) = 0;
		data<wxInt32>(shPorts) = 11;
		data<wxInt32>(shMinPlayers) = 5;
		data<wxInt32>(shMaxPlayers) = 6;
		data<wxInt32>(shChiplessTiles) = 20;
		data<wxInt32>(shChiplessChips) = 7;

		data<IntArray>(shRollPlaces).resize(6);
		data<IntArray>(shRollPlaces)[0] = 15;
		data<IntArray>(shRollPlaces)[1] = 48;
		data<IntArray>(shRollPlaces)[2] = 84;
		data<IntArray>(shRollPlaces)[3] = 86;
		data<IntArray>(shRollPlaces)[4] = 53;
		data<IntArray>(shRollPlaces)[5] = 17;

		data<Data::IntHash>(shRandomResources)[shTimber] = 6;
		data<Data::IntHash>(shRandomResources)[shWheat] = 6;
		data<Data::IntHash>(shRandomResources)[shOre] = 5;
		data<Data::IntHash>(shRandomResources)[shClay] = 5;
		data<Data::IntHash>(shRandomResources)[shSheep] = 6;
		data<Data::IntHash>(shRandomResources)[shDesert] = 2;
		data<Data::IntHash>(shRandomPorts)[shPort3] = 6;
		data<Data::IntHash>(shChiplessResources)[shTimber] = 4;
		data<Data::IntHash>(shChiplessResources)[shWheat] = 4;
		data<Data::IntHash>(shChiplessResources)[shOre] = 4;
		data<Data::IntHash>(shChiplessResources)[shClay] = 4;
		data<Data::IntHash>(shChiplessResources)[shSheep] = 4;
		data<Data::IntHash>(shChiplessChipArray)[sh2] = 1;
		data<Data::IntHash>(shChiplessChipArray)[sh3] = 1;
		data<Data::IntHash>(shChiplessChipArray)[sh4] = 1;
		data<Data::IntHash>(shChiplessChipArray)[sh5] = 1;
		data<Data::IntHash>(shChiplessChipArray)[sh9] = 1;
		data<Data::IntHash>(shChiplessChipArray)[sh10] = 1;
		data<Data::IntHash>(shChiplessChipArray)[sh11] = 1;
	
		Map::CreateMap(*this, 11, 9, boost::bind(&GreaterCatan_V_VI::SetTileData, 
			this, _1));
	}

private:
	void SetTileData(const wxInt32 index)
	{
		tile<wxInt32>(shSecondary, index) = FALSE;
		tile<wxInt32>(shInitial, index) = FALSE;
		tile<wxInt32>(shChipless, index) = FALSE;

		//set the tile type
		switch(index)
		{
		case 15: case 16: case 17: case 26: case 27: case 28: case 29: case 37:
		case 38: case 39: case 40: case 41: case 48: case 49: case 50: case 51:
		case 52: case 53: case 60: case 61: case 62: case 63: case 64: case 72:
		case 73: case 74: case 75: case 84: case 85: case 86:
			tile<HashString>(shTileType, index) = shRandom;
			tile<wxInt32>(shInitial, index) = TRUE;

			break;
		//ports
		case 3: case 6: case 25: case 30: case 36: case 54: case 59: case 76:
		case 87: case 95: case 97:
			tile<HashString>(shTileType, index) = shPort;

			if(25 == index) tile<wxInt32>(shPortSide, index) = 4;
			if(36 == index) tile<wxInt32>(shPortSide, index) = 3;
			if(97 == index) tile<wxInt32>(shPortSide, index) = 5;
			
			break;
		//chitless islands
		case 12: case 13: case 19: case 20: case 21: case 24: case 31: case 32:
		case 35: case 44: case 55: case 58: case 66: case 67: case 70: case 78:
		case 81: case 82: case 89: case 90:
			tile<HashString>(shTileType, index) = shChipless;
			tile<wxInt32>(shSecondary, index) = TRUE;

			break;
		default:
			break;
		}
	}
};

IMPLEMENT_OFFICIAL_MAP(GreaterCatan_V_VI);

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



