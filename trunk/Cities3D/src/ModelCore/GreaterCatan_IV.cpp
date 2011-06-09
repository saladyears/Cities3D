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
class GreaterCatan_IV : public GreaterCatanBase
{
public:
	GreaterCatan_IV()
	{
		mName = swTheBigIslandIV;
		mInternalName = swTheBigIslandIVInternal;

		data<wxInt32>(shID) = MapGreaterCatan_IV;
		data<wxInt32>(shRandomTiles) = 19;
		data<wxInt32>(shRandomChips) = 0;
		data<wxInt32>(shPorts) = 9;
		data<wxInt32>(shMinPlayers) = 4;
		data<wxInt32>(shMaxPlayers) = 4;
		data<wxInt32>(shChiplessTiles) = 18;
		data<wxInt32>(shChiplessChips) = 7;

		data<IntArray>(shRollPlaces).resize(6);
		data<IntArray>(shRollPlaces)[0] = 35;
		data<IntArray>(shRollPlaces)[1] = 55;
		data<IntArray>(shRollPlaces)[2] = 77;
		data<IntArray>(shRollPlaces)[3] = 79;
		data<IntArray>(shRollPlaces)[4] = 59;
		data<IntArray>(shRollPlaces)[5] = 37;

		data<Data::IntHash>(shRandomResources)[shTimber] = 4;
		data<Data::IntHash>(shRandomResources)[shWheat] = 4;
		data<Data::IntHash>(shRandomResources)[shOre] = 3;
		data<Data::IntHash>(shRandomResources)[shClay] = 3;
		data<Data::IntHash>(shRandomResources)[shSheep] = 4;
		data<Data::IntHash>(shRandomResources)[shDesert] = 1;
		data<Data::IntHash>(shRandomPorts)[shPort3] = 4;
		data<Data::IntHash>(shChiplessResources)[shTimber] = 4;
		data<Data::IntHash>(shChiplessResources)[shWheat] = 4;
		data<Data::IntHash>(shChiplessResources)[shOre] = 3;
		data<Data::IntHash>(shChiplessResources)[shClay] = 3;
		data<Data::IntHash>(shChiplessResources)[shSheep] = 4;
		data<Data::IntHash>(shChiplessChipArray)[sh2] = 1;
		data<Data::IntHash>(shChiplessChipArray)[sh3] = 1;
		data<Data::IntHash>(shChiplessChipArray)[sh4] = 1;
		data<Data::IntHash>(shChiplessChipArray)[sh5] = 1;
		data<Data::IntHash>(shChiplessChipArray)[sh9] = 1;
		data<Data::IntHash>(shChiplessChipArray)[sh10] = 1;
		data<Data::IntHash>(shChiplessChipArray)[sh11] = 1;

		Map::CreateMap(*this, 10, 9, boost::bind(&GreaterCatan_IV::SetTileData, 
			this, _1));
	}

private:
	void SetTileData(const wxInt32 index)
	{
		//set default values so that all data pieces are initialized
		tile<wxInt32>(shSecondary, index) = FALSE;
		tile<wxInt32>(shInitial, index) = FALSE;
		tile<wxInt32>(shChipless, index) = FALSE;

		//set the tile type
		switch(index)
		{
		case 35: case 36: case 37: case 45: case 46: case 47: case 48: case 55:
		case 56: case 57: case 58: case 59: case 66: case 67: case 68: case 69:
		case 77: case 78: case 79:
			//set random type
			tile<HashString>(shTileType, index) = shRandom;

			//set the initial placement to TRUE for starting land pieces
			tile<wxInt32>(shInitial, index) = TRUE;

			break;
		//ports
		case 25: case 27: case 34: case 49: case 54: case 70: case 76: case 88:
		case 90:
			//set port
			tile<HashString>(shTileType, index) = shPort;
			break;
		//chitless islands
		case 11: case 12: case 14: case 15: case 17: case 18: case 19: case 22:
		case 28: case 29: case 32: case 40: case 53: case 61: case 64: case 74:
		case 75: case 82:
			tile<HashString>(shTileType, index) = shChipless;
			tile<wxInt32>(shSecondary, index) = TRUE;
			break;
		default:
			break;
		}
	}
};

IMPLEMENT_OFFICIAL_MAP(GreaterCatan_IV);

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



