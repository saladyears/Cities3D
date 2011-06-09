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
#include "LogicHashMixin.h"
#include "RuleSetDatabase.h"
#include "KnightObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicIsCardPlayableSaboteur
// 
// Determines whether the Saboteur card is playable right now.
//
// Derived From:
//     <Logic>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
// Mixin To:
//     <LogicIsCardPlayable>
//
class LogicIsCardPlayableSaboteur : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		bool canPlay = false;

		// The Saboteur is playable if any other player has a city that does
		// not have a metropolis on it.
		wxInt32 curPlayer = current();
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			if(i != curPlayer)
			{
				// Get their city and metropolis count.
				size_t cities = 
					playerGame(i).getCornerObjects(shCities).size();
				size_t metropolis =
					playerGame(i).getCornerObjects(shMetropolis).size();

				if(cities > metropolis)
				{
					canPlay = true;
					break;
				}
			}
		}
		
		output = DataObject(canPlay);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicIsCardPlayableSaboteur, Saboteur,
							LogicIsCardPlayableCities, CITIES)
