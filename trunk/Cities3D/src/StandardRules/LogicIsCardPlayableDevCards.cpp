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
// Class: LogicIsCardPlayableDevCards
// 
// Determines whether the given card is playable right now.
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
//     <LogicIsCardPlayable>
//
class LogicIsCardPlayableDevCards : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		const HashString& card = input.read<HashString>();

		// The card is playable only if no other card has been played this turn
		// or it is a Victory Point card, which is always playable.
		bool ret = ((!playerGameData<wxInt32>(shPlayedCard)) ||
					(shVictoryPoint == card));

		output = DataObject(ret);
	}
};

IMPLEMENT_LOGIC_MIXIN(LogicIsCardPlayableDevCards, LogicIsCardPlayable, 
					  DEVCARDS)
