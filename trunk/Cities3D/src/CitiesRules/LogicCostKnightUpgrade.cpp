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
// Class: LogicCostKnightUpgrade
// 
// Returns the cost in resources to upgrade a knight.
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
class LogicCostKnightUpgrade : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		typedef std::map<HashString, wxInt32> ResourceMap;
		ResourceMap resources;

		// A knight upgrade costs 1 ore and 1 sheep, unless it is free.
		wxInt32 free = playerGameData<wxInt32>(shFreeKnightUpgrades);
		if(0 == free)
		{
			resources[shOre] = 1;
			resources[shSheep] = 1;
		}

		output = DataObject(resources);
	}
};

IMPLEMENT_LOGIC(LogicCostKnightUpgrade, CITIES)
