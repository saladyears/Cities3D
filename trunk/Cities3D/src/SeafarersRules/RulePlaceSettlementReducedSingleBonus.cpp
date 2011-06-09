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
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePlaceSettlementReducedSingleBonus
// 
// Handles moving extra points to the settlement when it replaces a reduced
// city.
//
// Derived From:
//     <Rule>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <SingleBonus>
//
// Mixin To:
//     <RulePlaceSettlementReduced>
//
class RulePlaceSettlementReducedSingleBonus : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxInt32 id = object.read<wxInt32>();
		
		wxInt32 player = current();
		if(2 <= object.numItems())
		{
			player = object.read<wxInt32>(1);
		}

		RULE.Execute(shRuleReplaceExtra, 
			DataObject(id, player, HashString(shSettlements)));
	}
};

IMPLEMENT_RULE_MIXIN(RulePlaceSettlementReducedSingleBonus, 
					 RulePlaceSettlementReduced, SINGLE_BONUS)
