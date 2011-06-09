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
// Class: RuleHandle7
// 
// Handles a 7 roll.
//
// Derived From:
//     <Rule>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// Game State:
//     TurnLose7
//
class RuleHandle7 : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		bool lose = false;

		// On a seven roll, examine each player and see if they have too many
		// cards.  If they do, set their LoseCards state.
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			wxInt32 allowed = playerGameData<wxInt32>(shAllowedCards, i);
			wxInt32 inhand = 0;

			const Data::IntHash& resources = 
				playerGameData<Data::IntHash>(shResources, i);
			Data::IntHash::const_iterator it, itEnd = resources.end();
			for(it = resources.begin(); it != itEnd; ++it)
			{
				inhand += it->second;
			}

			// Set their state.
			if(inhand > allowed)
			{
				playerGameData<wxInt32>(shLoseCards, i) = TRUE;
				lose = true;
			}
		}

		// If anyone loses cards, play the sound.
		if(true == lose)
		{
			RULE.Execute(shRulePlaySound, DataObject(SOUND_LOSE_ON_7));
		}

		// Now handle any card losing.
		gameData<HashString>(shState) = shTurnLose7;
		RULE.Execute(shRuleRestartTurnLose7, DataObject());
	}
};

IMPLEMENT_RULE(RuleHandle7, STANDARD)
