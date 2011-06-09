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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicIsCardPlayableEngineer
// 
// Determines whether the Engineer card is playable right now.
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
class LogicIsCardPlayableEngineer : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		bool canPlay = false;

		// To play the engineer card, the player must have at least one city
		// on the board with no city walls and a city walls in stock.
		if(0 < playerGameData<wxInt32>(shStockWalls))
		{
			const PlayerGame::CornerObjectArray& cities =
				playerGame().getCornerObjects(shCities);
			const PlayerGame::CornerObjectArray& walls =
				playerGame().getCornerObjects(shWalls);

			if(cities.size() > walls.size())
			{
				canPlay = true;
			}
		}

		output = DataObject(canPlay);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicIsCardPlayableEngineer, Engineer,
							LogicIsCardPlayableCities, CITIES)
