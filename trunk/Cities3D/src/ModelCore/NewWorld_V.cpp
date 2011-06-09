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
class NewWorld_V : public NewWorldBase
{
public:
	NewWorld_V()
	{
		mName = swANewWorldV;
		mInternalName = swANewWorldVInternal;

		data<wxInt32>(shID) = MapNewWorld_V;
		data<wxInt32>(shPorts) = 10;
		data<wxInt32>(shMinPlayers) = 5;
		data<wxInt32>(shMaxPlayers) = 5;
		data<wxInt32>(shRandomTiles) = 53;
		data<wxInt32>(shRandomChips) = 27;
		data<wxInt32>(shAllowedCards) = 9;
		
		data<Data::IntHash>(shRandomResources)[shTimber] = 6;
		data<Data::IntHash>(shRandomResources)[shWheat] = 5;
		data<Data::IntHash>(shRandomResources)[shOre] = 5;
		data<Data::IntHash>(shRandomResources)[shClay] = 5;
		data<Data::IntHash>(shRandomResources)[shSheep] = 6;
		data<Data::IntHash>(shRandomResources)[shOcean] = 26;
		data<Data::IntHash>(shRandomPorts)[shPort3] = 5;
		data<Data::IntHash>(shRandomChipArray)[sh2] = 2;
		data<Data::IntHash>(shRandomChipArray)[sh3] = 3;
		data<Data::IntHash>(shRandomChipArray)[sh4] = 3;
		data<Data::IntHash>(shRandomChipArray)[sh5] = 3;
		data<Data::IntHash>(shRandomChipArray)[sh6] = 3;
		data<Data::IntHash>(shRandomChipArray)[sh8] = 2;
		data<Data::IntHash>(shRandomChipArray)[sh9] = 4;
		data<Data::IntHash>(shRandomChipArray)[sh10] = 3;
		data<Data::IntHash>(shRandomChipArray)[sh11] = 3;
		data<Data::IntHash>(shRandomChipArray)[sh12] = 1;

		Map::CreateMap(*this, 9, 9, boost::bind(&NewWorld_V::SetTileData, 
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
		case 18: case 27: case 37: case 46: case 56: case 65: case 75: case 84:
		case 83: case 82: case 81: case 80:	case 79: case 78: case 77: case 76:
		case 66: case 57: case 47: case 38: case 28: case 19: case 9:
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

IMPLEMENT_OFFICIAL_MAP(NewWorld_V);

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



