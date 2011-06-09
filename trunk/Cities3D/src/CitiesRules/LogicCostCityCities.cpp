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
// Class: LogicCostCityCities
// 
// Returns the cost in resources to build a city.
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
class LogicCostCityCities : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		typedef std::map<HashString, wxInt32> ResourceMap;
		ResourceMap resources;

		// A city costs 3 ore and 2 wheat, unless it is cheaper.
		wxInt32 free = playerGameData<wxInt32>(shFreeCities);
		if(0 == free)
		{
			resources[shOre] = 3;
			resources[shWheat] = 2;
		}
		else
		{
			resources[shOre] = 2;
			resources[shWheat] = 1;
		}

		output = DataObject(resources);
	}
};

IMPLEMENT_LOGIC_REPLACE(LogicCostCityCities, CITIES, LogicCostCity)
