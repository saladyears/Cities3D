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
#include "KnightObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicKnightCounts
// 
// Determines who winners or losers are when the barbarian hits.
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
class LogicKnightCounts : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		typedef std::set<wxInt32> IntSet;
		IntSet results;
		
		wxInt32 players = numPlayers();
		IntArray active;
		active.resize(players, 0);

		IntArray city;
		city.resize(players, 0);

		IntArray metropolis;
		metropolis.resize(players, 0);

		size_t totalCities = 0;
		size_t totalKnights = 0;
		wxInt32 maxKnights = 0;

		// Count up active knights and total cities for each player.
		for(wxInt32 i = 0; i < players; ++i)
		{
			const PlayerGame::CornerObjectArray& cities =
				playerGame(i).getCornerObjects(shCities);
			city[i] = cities.size();
			totalCities += city[i];

			const PlayerGame::CornerObjectArray& metropolises =
				playerGame(i).getCornerObjects(shMetropolis);
			metropolis[i] = metropolises.size();

			const PlayerGame::CornerObjectArray& knights =
				playerGame(i).getCornerObjects(shKnights);

			PlayerGame::CornerObjectArray::const_iterator it,
				itEnd = knights.end();
			for(it = knights.begin(); it != itEnd; ++it)
			{
				KnightObject* knight = static_cast<KnightObject*>(it->get());
				if(true == knight->active())
				{
					active[i] += knight->strength();
				}
			}

			if(active[i] > maxKnights)
			{
				maxKnights = active[i];
			}
			
			totalKnights += active[i];
		}

		bool winners = (totalKnights >= totalCities);

		// Now determine who winners and losers are.  
		if(true == winners)
		{
			// Winners.  Any player that matches the maximum active is a 
			// winner.
			if(0 < totalCities)
			{
				for(wxInt32 i = 0; i < players; ++i)
				{
					if(active[i] == maxKnights)
					{
						results.insert(i);
					}
				}
			}

			// Note that it is possible for there to be no winners, even though
			// the barbarians are stopped.  This happens when there are no
			// cities left on the board.
		}
		else
		{
			// Losers.  Players with no cities, and players who have all their 
			// cities protected by metroplises are not penalized.

			// Find all players that match the lowest, and add them to the
			// list, as long as they do not meet the exception criteria.
			wxInt32 lowest = active[0];
			for(wxInt32 i = 0; i < players; ++i)
			{
				// Only players with cities that aren't metropolises can be
				// losers.
				if( (0 < city[i]) &&
					(city[i] > metropolis[i]))
				{
					if(active[i] < lowest)
					{
						lowest = active[i];
						results.clear();
						results.insert(i);
					}
					else if(active[i] == lowest)
					{
						results.insert(i);
					}
				}
			}

			// Note that it is possible for there to be no losers.  This would
			// be extremely rare, but any situation where there are more 
			// metropolises on the board than active knights, and the players
			// without metropolises have no knights would result in no losers.
		}

		output = DataObject(winners, results);
	}
};

IMPLEMENT_LOGIC(LogicKnightCounts, CITIES)
