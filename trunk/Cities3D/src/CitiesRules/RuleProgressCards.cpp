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
#include "RNG.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleProgressCards
// 
// Doles out progress cards to people.
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
class RuleProgressCards : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// Progress cards are dealt in the following manner:
		//
		// 1.  Start with the current player and continue through all players.
		// 2.  If the player has an upgrade level equal to one less than the
		//     red die (die 2), they receive a progress card for the given 
		//     type, if there are any progress cards left to give.
		
		wxInt32 neededLevel = gameData<wxInt32>(shDie2) - 1;
		wxInt32 thirdDie = gameData<wxInt32>(shDie3);
		
		HashString type;
		switch(thirdDie)
		{
			case 4:
				type = shTrade;
				break;
			case 5:
				type = shPolitics;
				break;
			case 6:
				type = shScience;
				break;
			default:
				wxASSERT(false);
				break;
		}
		HashString levelType = HashString::Format(shStringLevel.c_str(), type.c_str());

		wxInt32 curPlayer = current();
		wxInt32 players = numPlayers();
		for(wxInt32 i = 0; i < players; ++i)
		{
			wxInt32 thisPlayer = (i + curPlayer) % players;
			wxInt32 playerLevel = playerGameData<wxInt32>(levelType, 
				thisPlayer);

			if( (0 < playerLevel) &&
				(playerLevel >= neededLevel))
			{
				RULE.Execute(shRuleProgressCardCommon, DataObject(thisPlayer,
					type));
			}
		}

		RULE.Execute(shRuleMarkerPostDiceRoll, DataObject());
	}
};

IMPLEMENT_RULE(RuleProgressCards, CITIES)
