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
class NewWorld_VI : public NewWorldBase
{
public:
	NewWorld_VI()
	{
		mName = swANewWorldVI;
		mInternalName = swANewWorldVIInternal;

		data<wxInt32>(shID) = MapNewWorld_VI;
		data<wxInt32>(shPorts) = 10;
		data<wxInt32>(shMinPlayers) = 6;
		data<wxInt32>(shMaxPlayers) = 6;
		data<wxInt32>(shRandomTiles) = 60;
		data<wxInt32>(shRandomChips) = 32;
		data<wxInt32>(shAllowedCards) = 9;
		
		data<Data::IntHash>(shRandomResources)[shTimber] = 7;
		data<Data::IntHash>(shRandomResources)[shWheat] = 6;
		data<Data::IntHash>(shRandomResources)[shOre] = 6;
		data<Data::IntHash>(shRandomResources)[shClay] = 6;
		data<Data::IntHash>(shRandomResources)[shSheep] = 7;
		data<Data::IntHash>(shRandomResources)[shOcean] = 28;
		data<Data::IntHash>(shRandomPorts)[shPort3] = 5;
		data<Data::IntHash>(shRandomChipArray)[sh2] = 2;
		data<Data::IntHash>(shRandomChipArray)[sh3] = 3;
		data<Data::IntHash>(shRandomChipArray)[sh4] = 4;
		data<Data::IntHash>(shRandomChipArray)[sh5] = 4;
		data<Data::IntHash>(shRandomChipArray)[sh6] = 3;
		data<Data::IntHash>(shRandomChipArray)[sh8] = 3;
		data<Data::IntHash>(shRandomChipArray)[sh9] = 4;
		data<Data::IntHash>(shRandomChipArray)[sh10] = 4;
		data<Data::IntHash>(shRandomChipArray)[sh11] = 3;
		data<Data::IntHash>(shRandomChipArray)[sh12] = 2;

		Map::CreateMap(*this, 10, 9, boost::bind(&NewWorld_VI::SetTileData, 
			this, _1));
	}

private:
	void SetTileData(const wxInt32 index)
	{
		tile<wxInt32>(shSecondary, index) = TRUE;
		
		switch(index)
		{
		//skip borders
		case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: 
		case 9:	case 10: case 21: case 31: case 42: case 52: case 63: case 73: 
		case 84: case 85: case 86: case 87: case 88:	case 89: case 90: 
		case 91: case 92: case 93: case 83: case 72: case 62: case 51: case 41:
		case 30: case 20:
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

IMPLEMENT_OFFICIAL_MAP(NewWorld_VI);

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



