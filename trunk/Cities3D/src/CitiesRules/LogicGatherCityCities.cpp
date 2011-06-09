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
#include "Logic.h"
#include "RuleSetDatabase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicGatherCityCities
// 
// Provides logic that determines how many resources are acquired for a 
// city being on a tile.
//
// Derived From:
//     <Logic>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
class LogicGatherCityCities : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		wxASSERT(3 <= input.numItems());

		const HashString &type = input.read<HashString>();
		wxInt32 id = input.read<wxInt32>(1);
		wxInt32 index = input.read<wxInt32>(2);

		// Make sure the city is not sabotaged.
		IntArray& cities = playerGameData<IntArray>(shSabotagedCities, index);
		bool sabotaged = (cities.end() != 
			std::find(cities.begin(), cities.end(), id));

		typedef std::map<HashString, wxInt32> ResourceMap;
		ResourceMap resources;
		
		// They get one resource and one commodity of certain types.
		if(shOre == type)
		{
			resources[type] = 1;
			if(false == sabotaged)
			{
				resources[shCoin] = 1;
			}
		}
		else if(shTimber == type)
		{
			resources[type] = 1;
			if(false == sabotaged)
			{
				resources[shPaper] = 1;
			}
		}
		else if(shSheep == type)
		{
			resources[type] = 1;
			if(false == sabotaged)
			{
				resources[shCloth] = 1;
			}
		}
		else
		{
			// Otherwise, they get two resources of the given type for having a
			// city.
			resources[type] = 2;
		}

		output = DataObject(resources);
	}
};

IMPLEMENT_LOGIC_REPLACE(LogicGatherCityCities, CITIES, LogicGatherCity)
