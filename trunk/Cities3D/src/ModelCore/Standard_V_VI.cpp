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
class Standard_V_VI : public StandardBase
{
public:
	Standard_V_VI()
	{
		mName = swStandardSettlersVI;
		mInternalName = swStandardSettlersVIInternal;

		//identifier
		data<wxInt32>(shID) = MapStandard_V_VI;
		data<wxInt32>(shRandomTiles) = 30;
		data<wxInt32>(shRandomChips) = 0;
		data<wxInt32>(shPorts) = 11;
		data<wxInt32>(shMinPlayers) = 5;
		data<wxInt32>(shMaxPlayers) = 6;
		data<wxInt32>(shLandTiles) = 30;
		data<wxInt32>(shSeaTiles) = 22;
		data<wxInt32>(shAllowedCards) = 9;

		data<IntArray>(shRollPlaces).resize(6);
		data<IntArray>(shRollPlaces)[0] = 21;
		data<IntArray>(shRollPlaces)[1] = 24;
		data<IntArray>(shRollPlaces)[2] = 27;
		data<IntArray>(shRollPlaces)[3] = 29;
		data<IntArray>(shRollPlaces)[4] = 16;
		data<IntArray>(shRollPlaces)[5] = 19;

		data<Data::IntHash>(shRandomResources)[shTimber] = 6;
		data<Data::IntHash>(shRandomResources)[shWheat] = 6;
		data<Data::IntHash>(shRandomResources)[shOre] = 5;
		data<Data::IntHash>(shRandomResources)[shClay] = 5;
		data<Data::IntHash>(shRandomResources)[shSheep] = 6;
		data<Data::IntHash>(shRandomResources)[shDesert] = 2;
		data<Data::IntHash>(shRandomPorts)[shPort3] = 5;
		data<Data::IntHash>(shRandomPorts)[shPortTimber] = 1;
		data<Data::IntHash>(shRandomPorts)[shPortWheat] = 1;
		data<Data::IntHash>(shRandomPorts)[shPortOre] = 1;
		data<Data::IntHash>(shRandomPorts)[shPortClay] = 1;
		data<Data::IntHash>(shRandomPorts)[shPortSheep] = 2;

		//resize the map pieces
		mTiles.resize(52);

		//set the map pieces
		for(wxInt32 i = 0; i < 52; i++)
		{
			//set the piece attributes
			mTiles[i].id(i);
			tile<wxInt32>(shIndex, i) = i;
			tile<wxInt32>(shInitial, i) = TRUE;

			//what kind of piece
			if(i < 30)
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
	}
};

IMPLEMENT_OFFICIAL_MAP(Standard_V_VI);

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



