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
#include "Controller.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleLargestArmyCheck
// 
// Determines if a new player has taken Largest Army and adjusts points and
// totals accordingly.
//
// Derived From:
//     <Rule>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <DevCards>
//
class RuleLargestArmyCheck : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// So, here's how the Largest Army algorithm works:
		//
		// 1.  If no player currently has Largest Army...
		//     a) and no one has more than 3 played soldiers, no one gets
		//        Largest Army.
		//     b) and any one player has more than 3 played soldiers, they get 
		//        Largest Army.
		// 2.  If someone already has Largest Army...
		//     a) and anyone else ties them in soldiers played, the original 
		//        player retains Largest Army.
		//     b) and any one person surpasses them and everyone else in 
		//        soldiers played, they get Largest Army.
		//

		// Gather all of the soldiers played for each player.
		wxInt32 players = numPlayers();
		wxInt32 current = gameData<wxInt32>(shLargestArmyPlayer);

		std::vector<wxInt32> soldiers(numPlayers(), 0);

		for(wxInt32 i = 0; i < players; ++i)
		{
			soldiers[i] = 
				playerGameData<Data::IntHash>(shPlayedCards, i)[shSoldier];
		}

		// Now that we have all the cards, figure out if Largest Army
		// changes.

		wxInt32 soldiersPlayed = 0;
		wxInt32 largestPlayer = -1;

		// No current holder.
		if(-1 == current)
		{
			// With no current holder, see if anyone is over 3 and the clear
			// leader (no ties).
			for(wxInt32 i = 0; i < players; ++i)
			{
				wxInt32 played = soldiers[i];

				if(3 <= played)
				{
					// A clear leader.
					if(soldiersPlayed < played)
					{
						soldiersPlayed = played;
						largestPlayer = i;
					}
					// A tie.
					else if(soldiersPlayed == played)
					{
						largestPlayer = -1;
					}
				}
			}
		}
		// Someone already has it.
		else
		{
			// Set the mark that must be beaten.
			largestPlayer = current;
			soldiersPlayed = soldiers[current];

			for(wxInt32 i = 0; i < players; ++i)
			{
				// Skip the current holder.
				if(i == current)
				{
					continue;
				}

				wxInt32 played = soldiers[i];

				// Look for a clear leader.
				if(soldiersPlayed < played)
				{
					soldiersPlayed = played;
					largestPlayer = i;
				}
			}
		}

		// If it has changed hands, take it away from the loser and award it
		// to the winner.
		if(current != largestPlayer)
		{
			// Take it away from the loser.
			if(-1 != current)
			{
				RULE.Execute(shRuleLoseLargestArmy, DataObject(current));
			}

			// Give it to the winner.
			if(-1 != largestPlayer)
			{
				RULE.Execute(shRuleGainLargestArmy, DataObject(largestPlayer));
			}
		}

		// Regardless of what happens, update every player.
		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), -1));
	}
};

IMPLEMENT_RULE(RuleLargestArmyCheck, DEVCARDS)
