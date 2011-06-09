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
#include "PlayerData.h"
#include "RuleSetDatabase.h"
#include "LogicBranchDefault.h"
#include "IRuleEngine.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicVolcano
// 
// Determines if the volcanoes erupt.
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
class LogicVolcano : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		// Iterate through the tiles and see if any Volcanoes erupted.
		wxInt32 die1 = gameData<wxInt32>(shDie1);
		wxInt32 die2 = gameData<wxInt32>(shDie2);
		wxInt32 roll = die1 + die2;
		wxInt32 tiles = numTiles();
		bool erupt = false;

		for(wxInt32 i = 0; i < tiles; ++i)
		{
			if( (roll == tile<wxInt32>(shRoll, i)) &&
				(shVolcano == tile<HashString>(shTileType, i)))
			{
				erupt = true;
				break;
			}
		}

		// If there are any volcanoes, erupt!
		if(true == erupt)
		{
			output = DataObject(shRuleVolcano);
		}
		else
		{
			gameData<wxInt32>(shVolcanoDie) = -1;
			output = DataObject(shRuleMarkerPostDieRoll);
		}
	}
};

IMPLEMENT_LOGIC(LogicVolcano, VOLCANO)

//---------------------------- BRANCH LOGIC     -----------------------------//

// Volcanoes are handled almost last.
IMPLEMENT_PRIORITY_BRANCH(VOLCANO, RuleMarkerPostDiceRoll, LogicVolcano, 5);
