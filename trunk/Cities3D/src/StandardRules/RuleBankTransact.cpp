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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleBankTransact
// 
// Executes a transaction with the bank for the given player.
//
// Derived From:
//     <Rule>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
class RuleBankTransact : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(3 <= object.numItems());

		wxInt32 player = object.read<wxInt32>();
		const HashString &resource = object.read<HashString>(1);
		wxInt32 amount = object.read<wxInt32>(2);
		
		wxInt32 realAmount = amount;

		// First, check to see if the bank has enough of the resource.
		wxInt32 &bank = gameData<Data::IntHash>(shBankResources)[resource];

		if(INT_MIN != bank)
		{
			wxInt32 realAmount = std::min(amount, bank);
			wxInt32 blockedAmount = amount - realAmount;
			
			// If they got blocked, report it.
			if(0 < blockedAmount)
			{
				RULE.Execute(shRuleBlockResources, DataObject(player, blockedAmount, resource));
			}
		}

		// Handle the transaction.
		RULE.Execute(shRuleAdjustResources, DataObject(player, realAmount, resource));

		// If the bank has an infinite amount, don't bother subtracting.
		if(INT_MIN != bank)
		{
			bank -= realAmount;
			wxASSERT(0 <= bank);
		}
	}
};

IMPLEMENT_RULE(RuleBankTransact, STANDARD)
