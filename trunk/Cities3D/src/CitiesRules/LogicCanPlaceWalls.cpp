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
#include "CornerObject.h"
#include "WallsObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicCanPlaceWalls
// 
// Determines the available board locations for a knight to be placed for 
// the current player.
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
// Logic Called:
//     <LogicIsValidWallsPlacement>
//
class LogicCanPlaceWalls : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		PlayerGame::CornerObjectArray available;

		// A player can place a city walls if they have a city with no walls
		// currently under it.
		const PlayerGame::CornerObjectArray& cities =
			playerGame().getCornerObjects(shCities);

		PlayerGame::CornerObjectArray::const_iterator it, itEnd = cities.end();
		for(it = cities.begin(); it != itEnd; ++it)
		{
			bool walls = false;

			// Test all things connected to this city to see if they are walls.
			CornerObjectPtr below = (*it)->below().lock();
			while(below)
			{
				if(NULL != dynamic_cast<WallsObject*>(below.get()))
				{
					walls = true; 
					break;
				}

				below = below->below().lock();
			}

			if(false == walls)
			{
				available.push_back((*it));
			}
		}

		// Return the final placement selections.
		output = DataObject(available);
	}
};

IMPLEMENT_LOGIC(LogicCanPlaceWalls, CITIES)
