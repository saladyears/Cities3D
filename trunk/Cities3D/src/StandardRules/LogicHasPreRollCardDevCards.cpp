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
// Class: LogicHasPreRollCardDevCards
// 
// Determines whether the player has a development card that can be played 
// before the dice are rolled.
//
// Derived From:
//     <Logic>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <DevCards>
//
// Mixin To:
//     <LogicHasPreRollCard>
//
class LogicHasPreRollCardDevCards : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		// A play can play any card before the dice are rolled, but only one,
		// unless it is a development card, in which case they can play many.
		Data::IntHash& cards = 
			playerGameData<Data::IntHash>(shPlayableCards);

		wxInt32 total = 0;
		Data::IntHash::iterator it, itEnd = cards.end();
		for(it = cards.begin(); it != itEnd; ++it)
		{
			total += it->second;
		}

		wxInt32 vp = cards[shVictoryPoint];

		// See if they have played a card.
		wxInt32 played = playerGameData<wxInt32>(shPlayedCard);

		bool hasCard = false;

		if( (0 < total) &&
			((FALSE == played) ||
			(0 < vp)))
		{
			hasCard = true;
		}

		output = DataObject(hasCard);
	}
};

IMPLEMENT_LOGIC_MIXIN(LogicHasPreRollCardDevCards, LogicHasPreRollCard, 
					  DEVCARDS)
