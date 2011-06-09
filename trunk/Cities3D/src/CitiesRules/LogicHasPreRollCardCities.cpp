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
// Class: LogicHasPreRollCardCities
// 
// Determines whether the player has a progress card that can be played 
// before the dice are rolled.
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
//     <LogicHasPreRollCard>
//
class LogicHasPreRollCardCities : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		// Although the only playable card is the Alchemist, we consider all 
		// Science cards to be preroll-playable, because otherwise it would be 
		// obvious that a player has an Alchemist.  If the player has already 
		// played an Alchemist card, then no more cards are playable pre-roll.
		bool canPlay = false;
		
		if(FALSE == playerGameData<wxInt32>(shAlchemist))
		{
			Data::IntHash& cards = 
				playerGameData<Data::IntHash>(shPlayableScienceCards);
			Data::IntHash::const_iterator it, itEnd = cards.end();
			for(it = cards.begin(); it != itEnd; ++it)
			{
				if (it->second > 0) 
				{
					canPlay = true;
					break;
				}
			}
		}
			
		output = DataObject(canPlay);
	}
};

IMPLEMENT_LOGIC_MIXIN(LogicHasPreRollCardCities, LogicHasPreRollCard, 
					  CITIES)
