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
#include "IUndoable.h"
#include "RuleSetDatabase.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRemoveSettlementRepeatingBonus
// 
// Handles moving extra points to the settlement when a city placement is
// undone.
//
// Derived From:
//     <Rule>
//     <IUndoable>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <RepeatingBonus>
//
// Mixin To:
//     <RulePlaceCityTUrn>
//
class RuleRemoveSettlementRepeatingBonus 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &)
	{
		// Do nothing here, the undo is what matters.
	}

	virtual void Unexecute(const DataObject &object)
	{
		wxInt32 id = object.read<wxInt32>();
		RULE.Execute(shRuleReplaceExtras, DataObject(id, current(), 
			shSettlements));
	}

	virtual bool CanUndo() const
	{
		// This should never stop an undo chain.
		return true;
	}
};

IMPLEMENT_RULE_MIXIN(RuleRemoveSettlementRepeatingBonus, RulePlaceCityTurn, 
					 REPEATING_BONUS)
