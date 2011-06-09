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
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleMetropolis
// 
// Checks to see if a metropolis needs to be placed.
//
// Derived From:
//     <Rule>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
class RuleMetropolis : public Rule
{
public:
	virtual void Execute(const DataObject& object)
	{
		// Here are the rules related to taking a metropolis:
		// 
		// 1.  If no one currently has the metropolis, the first player to
		//     level 4 takes it.
		// 2.  If someone already has the metropolis, it can only be taken by
		//     a player who reaches level 5 before the current holder of the
		//     metropolis does.
		//
		const HashString& type = object.read<HashString>();
		HashString level = HashString::Format(shStringLevel.c_str(), 
            type.c_str());
		HashString player = HashString::Format(shStringPlayer.c_str(), 
            type.c_str());

		wxInt32 playerLevel = playerGameData<wxInt32>(level);

		// Only do something if we're at level 4 or higher.
		if(4 <= playerLevel)
		{
			// See if anyone else currently holds it.
			wxInt32 otherIndex = gameData<wxInt32>(player);
			wxInt32 otherLevel = 0;

			// If someone else has it, see if we beat them.
			if(-1 != otherIndex)
			{
				otherLevel = playerGameData<wxInt32>(level, otherIndex);
			}

			// If we beat it, the player gets to place the metropolis.
			if(playerLevel > otherLevel)
			{
				gameData<HashString>(shState) = shMetropolis;
				gameData<HashString>(shMetropolisType) = type;

				// If a new metropolis is being placed, this rule can't be
				// undone.
				RULE.Execute(shRuleBreakUndo, DataObject());
				RULE.Execute(shRuleRestartMetropolis, DataObject());
			}
		}
	}
};

IMPLEMENT_RULE(RuleMetropolis, CITIES)
