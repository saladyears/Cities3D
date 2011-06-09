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
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicCanBuyDevCardTurn
// 
// Determines whether a dev card can be bought during a player's turn.
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
//     <LogicCanBuyDevCard>
//
// Logic Called:
//     <LogicCanPurchaseDevCard>
//
class LogicCanBuyDevCardTurn : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		// There are two conditions that must be met in order for a player to
		// be allowed to buy a dev card.
		// 
		// 1) There must be at least one card left in the bank.
		// 2) They must have the resources to purchase a card.
		//
		// Condition 1 is static and cannot be affected by any other rulesets.
		// Condition 2 is static and cannot be affected by any other rulesets.

		bool canBuild = false;
		wxInt32 trades = 0;

		// Count how many dev cards are left.
		wxInt32 cards = 0;
		const Data::IntHash& devCards = 
			gameData<Data::IntHash>(shBankDevCards);
		Data::IntHash::const_iterator it, itEnd = devCards.end();
		for(it = devCards.begin(); it != itEnd; ++it)
		{
			cards += it->second;
		}

		// Condition 1.
		if(0 < cards)
		{
			// Condition 2.
			RULE.Decide(shLogicCanPurchaseDevCard, input, output);
			wxASSERT(2 <= output.numItems());
			canBuild = output.read<bool>();

			if(true == canBuild)
			{
				trades = output.read<wxInt32>(1);
			}
		}

		output = DataObject(canBuild, trades);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCanBuyDevCardTurn, Turn, LogicCanBuyDevCard, 
							DEVCARDS)
