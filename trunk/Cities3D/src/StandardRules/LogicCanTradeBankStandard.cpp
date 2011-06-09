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
// Class: LogicCanTradeBankStandard
// 
// Determines whether the player can trade with the bank on their turn.
//
// Derived From:
//     <Logic>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// Mixin To:
//     <LogicCanTradeBank>
//
// Logic Called:
//     <LogicIsResourceBankTradeable>
//
class LogicCanTradeBankStandard : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		// In order for a player to be able to trade with the bank, they 
		// must be able to trade any one of their resources in.

		bool canTrade = false;

		if(shTurn == gameData<HashString>(shState))
		{
			const Data::IntHash &resources = 
				playerGameData<Data::IntHash>(shResources);

			// Test each resource for tradability.
			Data::IntHash::const_iterator it, itEnd = resources.end();
			for(it = resources.begin(); it != itEnd; ++it)
			{
				DataObject input(it->first, it->second);
				output.reset();
				RULE.Decide(shLogicIsResourceBankTradeable, input, output);

				if(true == output.read<bool>())
				{
					canTrade = true;
					break;
				}
			}
		}

		output = DataObject(canTrade);
	}
};

IMPLEMENT_LOGIC_MIXIN(LogicCanTradeBankStandard, LogicCanTradeBank, STANDARD)
