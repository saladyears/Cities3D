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
// Class: RulePlayProgressCardStart
// 
// Common progress card code.
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
class RulePlayProgressCardStart : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		// Store off the current state.
		const HashString& state = gameData<HashString>(shState);
		gameData<HashString>(shOldState) = state;

		// Return this card to the deck.
		const HashString& card = object.read<HashString>();
		const HashString& type = object.read<HashString>(1);

		HashString bankType = HashString::Format(shBankStringCards.c_str(), 
            type.c_str());
		HashStringArray& deck = gameData<HashStringArray>(bankType);

		// This hurts.
		deck.insert(deck.begin(), card);

		// Remove this card from the player's hand.
		HashString handType = HashString::Format(shPlayableStringCards.c_str(), 
            type.c_str());
		Data::IntHash& cards = playerGameData<Data::IntHash>(handType);

		wxInt32& count = cards[card];
		wxASSERT(0 < count);
		--count;

		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), current()));
	}
};

IMPLEMENT_RULE(RulePlayProgressCardStart, CITIES)
