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
// Class: LogicIsCardPlayableArsonist
// 
// Determines whether the Arsonist card is playable right now.
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
class LogicIsCardPlayableArsonist : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		// Determine if there are any players with at least as many VPs as this player.
		// If there are not, they cannot play the card.
		wxInt32 points = playerGameData<wxInt32>(shPoints);
		wxInt32 curPlayer = current();
		bool canPlay = false;

		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			if(curPlayer != i && points <= playerGameData<wxInt32>(shPoints, i))
			{
				// The player must also have cards to lose.
				wxInt32 total = 0;
				const Data::IntHash& resources = 
					playerGameData<Data::IntHash>(shResources, i);
				Data::IntHash::const_iterator it, itEnd = resources.end();
				for(it = resources.begin(); it != itEnd; ++it)
				{
					total += it->second;
				}

				// The player can only lose cards if it has
				// at least 2
				if(1 < total)
				{
					canPlay = true;
					break;
				}
			}
		}

		output = DataObject(canPlay);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicIsCardPlayableArsonist, Arsonist,
							LogicIsCardPlayableCities, CITIES)
