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
#include "IUndoable.h"
#include "RuleSetDatabase.h"
#include "Controller.h"
#include "IRuleEngine.h"
#include "SideObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleLongestRoadCheck
// 
// Determines if a new player has taken Longest Road and adjusts points and
// totals accordingly.
//
// Derived From:
//     <Rule>
//     <IUndoable>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
class RuleLongestRoadCheck 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
		// Only execute if we're supposed to.
		if(false == object.read<bool>())
		{
			return;
		}

		// So, here's how the Longest Road algorithm works:
		//
		// 1.  If no player currently has Longest Road...
		//     a) and no one has more than 5 connected roads, no one gets
		//        Longest Road.
		//     b) and multiple players have more than 5 connected roads, but
		//        are tied, no one gets Longest Road.
		//     c) and any one player has more than 5 connected roads and is in
		//        the lead, they get Longest Road.
		// 2.  If someone already has Longest Road...
		//     a) and anyone else ties them in length, the original player
		//        retains Longest Road.
		//     b) and multiple players surpass them in length, but are tied,
		//        Longest Road reverts to unclaimed.
		//     c) and any one person surpasses them and everyone else in 
		//        length, they get Longest Road.
		//     d) and the player drops below 5 in road length, loses Longest
		//        Road, with Longest Road going to the appropriate player
		//        based on the previous criteria.
		//

		// With that in mind, run through each player, retrieving their
		// longest chain of roads.
		wxInt32 players = numPlayers();
		wxInt32 current = gameData<wxInt32>(shLongestRoadPlayer);

		std::vector<wxInt32> lengths(numPlayers(), 0);
		std::vector<PlayerGame::SideObjectArray> chains(numPlayers());

		PlayerGame::SideObjectArray longestChain;

		for(wxInt32 i = 0; i < players; ++i)
		{
			DataObject input(i), output;
			RULE.Decide(shLogicRoadLength, input, output);

			lengths[i] = output.read<wxInt32>();
			chains[i] = output.read<PlayerGame::SideObjectArray>(1);

			// While we're going through each player, clear any Longest Road
			// highlighting.
			PlayerGame::SideObjectArray objects;
			playerGame(i).getAllSideObjects(objects);

			PlayerGame::SideObjectArray::const_iterator it, 
				itEnd = objects.end();
			for(it = objects.begin(); it != itEnd; ++it)
			{
				(*it)->longest(false);
			}
		}

		// Now that we have all the lengths, figure out if Longest Road
		// changes.

		wxInt32 longestLength = 0;
		wxInt32 longestPlayer = -1;

		// In the special case where the original player drops below 5 in road
		// length, clear them out first and then apply the normal rules.
		if( (-1 != current) &&
			(5 > lengths[current]))
		{
			RULE.Execute(shRuleLoseLongestRoad, DataObject(current));
			current = -1;
		}

		// No current holder.
		if(-1 == current)
		{
			// With no current holder, see if anyone is over 5 and the clear
			// leader (no ties).
			for(wxInt32 i = 0; i < players; ++i)
			{
				wxInt32 len = lengths[i];

				if(5 <= len)
				{
					// A clear leader.
					if(longestLength < len)
					{
						longestLength = len;
						longestPlayer = i;
					}
					// A tie.
					else if(longestLength == len)
					{
						longestPlayer = -1;
					}
				}
			}
		}
		// Someone already has it.
		else
		{
			// Set the mark that must be beaten.
			longestPlayer = current;
			longestLength = lengths[current];

			for(wxInt32 i = 0; i < players; ++i)
			{
				// Skip the current holder.
				if(i == current)
				{
					continue;
				}

				wxInt32 len = lengths[i];

				// Look for a clear leader.
				if(longestLength < len)
				{
					longestLength = len;
					longestPlayer = i;
				}
				// Here's where it gets interesting.  If they tie the original 
				// player, it counts for nothing.  However, if they tie a new 
				// player who has already surpassed the original player, then 
				// it's a deadlock, and no one is the longest player.
				else if((longestLength == len) &&
						(longestPlayer != current))
				{
					longestPlayer = -1;
				}
			}
		}

		// Always update the highlighting on the longest road.
		if(-1 != longestPlayer)
		{
			PlayerGame::SideObjectArray& chain = chains[longestPlayer];

			// Mark all of the longest chain.
			PlayerGame::SideObjectArray::const_iterator it, 
				itEnd = chain.end();
			for(it = chain.begin(); it != itEnd; ++it)
			{
				(*it)->longest(true);
			}
		}

		// If it has changed hands, take it away from the loser and award it
		// to the winner.
		if(current != longestPlayer)
		{
			// Take it away from the loser.
			if(-1 != current)
			{
				RULE.Execute(shRuleLoseLongestRoad, DataObject(current));
			}

			// Give it to the winner.
			if(-1 != longestPlayer)
			{
				RULE.Execute(shRuleGainLongestRoad, DataObject(longestPlayer));
			}
		}

		// Regardless of what happens, update every player.
		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), -1));
	}

	virtual void Unexecute(const DataObject &object)
	{
		// Since we can only Unexecute if Longest Road has not changed, simply
		// rerun the Execute, which will update road lengths.
		Execute(object);
	}

	virtual bool CanUndo() const
	{
		// Calling sub-rules RuleLoseLongestRoad or RuleGainLongestRoad
		// halts the undo chain, so we can always return true here.
		return true;
	}
};

IMPLEMENT_RULE(RuleLongestRoadCheck, STANDARD)
