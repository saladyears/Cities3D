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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicCanShowCardCities
// 
// Determines whether the player can bring up the dialog showing their progress
// cards.
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
//     <LogicCanShowCard>
//
class LogicCanShowCardCities : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		// A player must just have any playable or purchased cards in order
		// to see them in the window.
        wxInt32 playerIndex = input.read<wxInt32>();

		bool canShow = false;
		
		if(-1 != playerIndex)
		{
			const HashString& state = gameData<HashString>(shState);

			// If we are pre roll, only the Alchemist card can be played, and only
			// if they haven't already played one.
			if(shPreRoll == state)
			{
				if(FALSE == playerGameData<wxInt32>(shAlchemist))
				{
					const Data::IntHash &hash = 
						playerGameData<Data::IntHash>(shPlayableScienceCards, playerIndex);

					Data::IntHash::const_iterator it = hash.find(shAlchemist);
					wxASSERT(hash.end() != it);

					if(0 < it->second)
					{
						canShow = true;
					}
				}
			}
			// During the turn all cards except the Alchemist may be played.
			else if(shTurn == state)
			{
				wxInt32 sum = 0;

				sum += sumHash(shPlayableTradeCards, playerIndex);
				sum += sumHash(shPlayablePoliticsCards, playerIndex);
				sum += sumHash(shPlayableScienceCards, playerIndex);

				if(0 < sum)
				{
					canShow = true;
				}		
			}
		}

		output = DataObject(canShow);
	}

private:
	wxInt32 sumHash(const HashString& type, wxInt32 playerIndex)
	{
		wxInt32 sum = 0;

		const Data::IntHash &hash = playerGameData<Data::IntHash>(type, playerIndex);

		Data::IntHash::const_iterator it, itEnd = hash.end();
		for(it = hash.begin(); it != itEnd; ++it)
		{
			const HashString& card = it->first;
			if(shAlchemist != card)
			{
				sum += it->second;
			}
		}

		return sum;
	}
};

IMPLEMENT_LOGIC_MIXIN(LogicCanShowCardCities, LogicCanShowCard, CITIES)
