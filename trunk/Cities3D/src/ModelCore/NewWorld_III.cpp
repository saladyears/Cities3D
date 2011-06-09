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
class NewWorld_III : public NewWorldBase
{
public:
	NewWorld_III()
	{
		mName = swANewWorldIII;
		mInternalName = swANewWorldIIIInternal;

		data<wxInt32>(shID) = MapNewWorld_III;
		data<wxInt32>(shPorts) = 9;
		data<wxInt32>(shMinPlayers) = 3;
		data<wxInt32>(shMaxPlayers) = 3;
		data<wxInt32>(shRandomTiles) = 39;
		data<wxInt32>(shRandomChips) = 20;
		
		data<Data::IntHash>(shRandomResources)[shTimber] = 4;
		data<Data::IntHash>(shRandomResources)[shWheat] = 4;
		data<Data::IntHash>(shRandomResources)[shOre] = 4;
		data<Data::IntHash>(shRandomResources)[shClay] = 4;
		data<Data::IntHash>(shRandomResources)[shSheep] = 4;
		data<Data::IntHash>(shRandomResources)[shOcean] = 19;
		data<Data::IntHash>(shRandomPorts)[shPort3] = 4;
		data<Data::IntHash>(shRandomChipArray)[sh2] = 1;
		data<Data::IntHash>(shRandomChipArray)[sh3] = 2;
		data<Data::IntHash>(shRandomChipArray)[sh4] = 2;
		data<Data::IntHash>(shRandomChipArray)[sh5] = 2;
		data<Data::IntHash>(shRandomChipArray)[sh6] = 2;
		data<Data::IntHash>(shRandomChipArray)[sh8] = 2;
		data<Data::IntHash>(shRandomChipArray)[sh9] = 3;
		data<Data::IntHash>(shRandomChipArray)[sh10] = 3;
		data<Data::IntHash>(shRandomChipArray)[sh11] = 2;
		data<Data::IntHash>(shRandomChipArray)[sh12] = 1;

		Map::CreateMap(*this, 7, 9, boost::bind(&NewWorld_III::SetTileData, 
			this, _1));
	}

private:
	void SetTileData(const wxInt32 index)
	{
		tile<wxInt32>(shSecondary, index) = TRUE;
		
		switch(index)
		{
		//skip borders
		case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 14: 
		case 21: case 29: case 36: case 44: case 51: case 59: case 66: case 65: 
		case 64: case 63: case 62: case 61:	case 60: case 52: case 45: case 37: 
		case 30: case 22: case 15: case 7: 
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

IMPLEMENT_OFFICIAL_MAP(NewWorld_III);

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



