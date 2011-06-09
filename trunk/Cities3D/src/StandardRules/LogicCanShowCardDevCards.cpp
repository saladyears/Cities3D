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
// Class: LogicCanShowCardDevCards
// 
// Determines whether the player can bring up the dialog showing their dev
// cards.
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
//     <LogicCanShowCard>
//
// Logic Called:
//     <LogicIsResourceBankTradeable>
//
class LogicCanShowCardDevCards : public Logic
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

			if( (shPreRoll == state) ||
				(shTurn == state))
			{
				const Data::IntHash &hash = 
					playerGameData<Data::IntHash>(shPurchasedCards, playerIndex);

				wxInt32 sum = 0;
				Data::IntHash::const_iterator it, itEnd = hash.end();
				for(it = hash.begin(); it != itEnd; ++it)
				{
					sum += it->second;
				}

				//playable cards
				const Data::IntHash &hash2 = 
					playerGameData<Data::IntHash>(shPlayableCards, playerIndex);

				itEnd = hash2.end();
				for(it = hash2.begin(); it != itEnd; ++it)
				{
					sum += it->second;
				}

				canShow = (0 < sum);
			}
		}

		output = DataObject(canShow);
	}
};

IMPLEMENT_LOGIC_MIXIN(LogicCanShowCardDevCards, LogicCanShowCard, DEVCARDS)
