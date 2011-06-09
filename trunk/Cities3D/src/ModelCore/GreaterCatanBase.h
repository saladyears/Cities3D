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
#pragma once

#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//
#include "OfficialMap.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//
class GreaterCatanBase : public OfficialMap
{
public:
	GreaterCatanBase()
	{
		data<wxInt32>(shPoints) = 18;
		data<wxInt32>(shStockShips) = 15;
		data<wxInt32>(shStockCities) = 8;
		
		data<Data::IntHash>(shRandomPorts)[shPortTimber] = 1;
		data<Data::IntHash>(shRandomPorts)[shPortWheat] = 1;
		data<Data::IntHash>(shRandomPorts)[shPortOre] = 1;
		data<Data::IntHash>(shRandomPorts)[shPortClay] = 1;
		data<Data::IntHash>(shRandomPorts)[shPortSheep] = 1;

		mRuleSets.push_back(std::make_pair(swSeafarersRules, 
			swSeafarers));
		mRuleSets.push_back(std::make_pair(swStandardRules, 
			swRandomTiles));
		mRuleSets.push_back(std::make_pair(swStandardRules, 
			swAroundTheBoard));
		mRuleSets.push_back(std::make_pair(swSeafarersRules, 
			swChiplessTiles));
	}
};

//---------------------------- PROTOTYPES       -----------------------------//

