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
#include "Rule.h"
#include "RuleSetDatabase.h"
#include "Map.h"
#include "GameEngine.h"
#include "UtilityMap.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleGeneratePorts
// 
// Provides a default implementation of placing random
// ports on the board.
//
// Derived From:
//     <Rule>
//
// Project:
//     <CoreRules>
//
// RuleSet:
//     <Core>
//
class RuleGeneratePorts : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		//this function takes the random port allocations in the map and 
		//assigns them to the places on the board that are marked as random 
		//ports
		HashStringArray values;

		//get the arrays ready
		wxInt32 size;
		size = UtilityMap::prepareDataArray(MAP.read<Data::IntHash>(
			shRandomPorts), values);

		//sanity
		wxASSERT(size == MAP.read<wxInt32>(shPorts));

		//search for port squares to set
		for(wxInt32 i = 0, j = 0; i < numTiles(); i++)
		{
			HashString &str = tile<HashString>(shTileType, i);
			
			//only set ports
			if(shPort == str)
			{
				str = values[j++];
			}
		}
	}
};

IMPLEMENT_RULE(RuleGeneratePorts, CORE);
