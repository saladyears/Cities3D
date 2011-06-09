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
#include "KnightObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicIsCardPlayableSpy
// 
// Determines whether the Spy card is playable right now.
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
class LogicIsCardPlayableSpy : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		// If any player has a progress card besides this player, the Spy
		// is playable.
		bool canPlay = false;
		wxInt32 curPlayer = current();

		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			if(i != curPlayer)
			{
				// The player must have any progress card.
				wxInt32 total = 0;
				total += CountCards(shPlayableTradeCards, i);
				total += CountCards(shPlayablePoliticsCards, i);
				total += CountCards(shPlayableScienceCards, i);
				
				if(0 < total)
				{
					canPlay = true;
					break;
				}
			}
		}

		output = DataObject(canPlay);
	}

private:
	wxInt32 CountCards(const HashString& type, wxInt32 player)
	{
		wxInt32 total = 0;

		const Data::IntHash& cards = 
			playerGameData<Data::IntHash>(type, player);
		Data::IntHash::const_iterator it, itEnd = cards.end();
		for(it = cards.begin(); it != itEnd; ++it)
		{
			total += it->second;
		}

		return total;
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicIsCardPlayableSpy, Spy,
							LogicIsCardPlayableCities, CITIES)
