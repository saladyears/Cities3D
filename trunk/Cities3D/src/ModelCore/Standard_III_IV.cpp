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
#include "StandardBase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
class Standard_III_IV : public StandardBase
{
public:
	Standard_III_IV()
	{
		mName = swStandardSettlersIV;
		mInternalName = swStandardSettlersIVInternal;

		data<wxInt32>(shID) = MapStandard_III_IV;
		data<wxInt32>(shRandomTiles) = 19;
		data<wxInt32>(shRandomChips) = 0;
		data<wxInt32>(shPorts) = 9;
		data<wxInt32>(shMinPlayers) = 3;
		data<wxInt32>(shMaxPlayers) = 4;
		data<wxInt32>(shLandTiles) = 19;
		data<wxInt32>(shSeaTiles) = 18;

		data<IntArray>(shRollPlaces).resize(6);
		data<IntArray>(shRollPlaces)[0] = 8;
		data<IntArray>(shRollPlaces)[1] = 10;
		data<IntArray>(shRollPlaces)[2] = 12;
		data<IntArray>(shRollPlaces)[3] = 14;
		data<IntArray>(shRollPlaces)[4] = 16;
		data<IntArray>(shRollPlaces)[5] = 18;

		data<Data::IntHash>(shRandomResources)[shTimber] = 4;
		data<Data::IntHash>(shRandomResources)[shWheat] = 4;
		data<Data::IntHash>(shRandomResources)[shOre] = 3;
		data<Data::IntHash>(shRandomResources)[shClay] = 3;
		data<Data::IntHash>(shRandomResources)[shSheep] = 4;
		data<Data::IntHash>(shRandomResources)[shDesert] = 1;
		data<Data::IntHash>(shRandomPorts)[shPort3] = 4;
		data<Data::IntHash>(shRandomPorts)[shPortTimber] = 1;
		data<Data::IntHash>(shRandomPorts)[shPortWheat] = 1;
		data<Data::IntHash>(shRandomPorts)[shPortOre] = 1;
		data<Data::IntHash>(shRandomPorts)[shPortClay] = 1;
		data<Data::IntHash>(shRandomPorts)[shPortSheep] = 1;

		//resize the map pieces
		mTiles.resize(37);

		//set the map pieces
		for(wxInt32 i = 0; i < 37; i++)
		{
			//set the piece attributes
			mTiles[i].id(i);
			tile<wxInt32>(shIndex, i) = i;
			tile<wxInt32>(shInitial, i) = TRUE;

			//what kind of piece
			if(i < 19)
			{
				tile<HashString>(shTileType, i) = shRandom;
			}
			else
			{
				tile<HashString>(shTileType, i) = shOcean;
				tile<wxInt32>(shBorder, i) = TRUE;
			}

			//set the coords
			StandardCoords(i);
		}

		//reset the -1 values that are for 5-6 player maps
		for(wxInt32 i = 19; i < 37; i++)
		{
			for(wxInt32 j = 0; j < 6; j++)
			{
				IntArray &sides = tile<IntArray>(shSides, i);

				if(37 <= sides[j])
				{
					sides[j] = -1;
				}
			}
		}
	}
};

IMPLEMENT_OFFICIAL_MAP(Standard_III_IV);

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



