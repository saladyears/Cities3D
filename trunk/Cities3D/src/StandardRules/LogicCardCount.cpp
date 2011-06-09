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
// Class: LogicCardCount
// 
// Logic that counts the number of cards the given player has in hand.
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
class LogicCardCount : public Logic
{
public:
	virtual void Decide(const DataObject& input, DataObject& output)
	{
		wxASSERT(1 <= input.numItems());
		wxInt32 index = input.read<wxInt32>();

		// Sum up their purchased cards and playable cards.
		const Data::IntHash &hash = 
			playerGameData<Data::IntHash>(shPurchasedCards, index);

		wxInt32 sum = 0;
		Data::IntHash::const_iterator it, itEnd(hash.end());
		for(it = hash.begin(); it != itEnd; ++it)
		{
			sum += it->second;
		}

		// Playable cards.
		const Data::IntHash &hash2 = 
			playerGameData<Data::IntHash>(shPlayableCards, index);
		
		itEnd = hash2.end();
		for(it = hash2.begin(); it != itEnd; ++it)
		{
			sum += it->second;
		}

		output = DataObject(sum);
	}
};

IMPLEMENT_LOGIC(LogicCardCount, DEVCARDS)
