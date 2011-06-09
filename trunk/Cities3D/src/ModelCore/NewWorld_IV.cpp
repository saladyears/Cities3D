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
#include "NewWorldBase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
class NewWorld_IV : public NewWorldBase
{
public:
	NewWorld_IV()
	{
		mName = swANewWorldIV;
		mInternalName = swANewWorldIVInternal;

		data<wxInt32>(shID) = MapNewWorld_IV;
		data<wxInt32>(shPorts) = 10;
		data<wxInt32>(shMinPlayers) = 4;
		data<wxInt32>(shMaxPlayers) = 4;
		data<wxInt32>(shRandomTiles) = 46;
		data<wxInt32>(shRandomChips) = 23;
		
		data<Data::IntHash>(shRandomResources)[shTimber] = 5;
		data<Data::IntHash>(shRandomResources)[shWheat] = 5;
		data<Data::IntHash>(shRandomResources)[shOre] = 4;
		data<Data::IntHash>(shRandomResources)[shClay] = 4;
		data<Data::IntHash>(shRandomResources)[shSheep] = 5;
		data<Data::IntHash>(shRandomResources)[shOcean] = 23;
		data<Data::IntHash>(shRandomPorts)[shPort3] = 5;
		data<Data::IntHash>(shRandomChipArray)[sh2] = 1;
		data<Data::IntHash>(shRandomChipArray)[sh3] = 3;
		data<Data::IntHash>(shRandomChipArray)[sh4] = 3;
		data<Data::IntHash>(shRandomChipArray)[sh5] = 3;
		data<Data::IntHash>(shRandomChipArray)[sh6] = 2;
		data<Data::IntHash>(shRandomChipArray)[sh8] = 2;
		data<Data::IntHash>(shRandomChipArray)[sh9] = 3;
		data<Data::IntHash>(shRandomChipArray)[sh10] = 3;
		data<Data::IntHash>(shRandomChipArray)[sh11] = 2;
		data<Data::IntHash>(shRandomChipArray)[sh12] = 1;

		Map::CreateMap(*this, 8, 9, boost::bind(&NewWorld_IV::SetTileData, 
			this, _1));
	}

private:
	void SetTileData(const wxInt32 index)
	{
		tile<wxInt32>(shSecondary, index) = TRUE;
		
		switch(index)
		{
		//skip borders
		case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: 
		case 16: case 24: case 33: case 41: case 50: case 58: case 67: case 75: 
		case 74: case 73: case 72: case 71:	case 70: case 69: case 68: case 59: 
		case 51: case 42: case 34: case 25: case 17: case 8: 
			tile<wxInt32>(shInitial, index) = FALSE;
			tile<wxInt32>(shSecondary, index) = FALSE;
			break;
		default:
			tile<HashString>(shTileType, index) = shRandom;
			tile<wxInt32>(shInitial, index) = TRUE;
			break;
		}
	}
};

IMPLEMENT_OFFICIAL_MAP(NewWorld_IV);

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



