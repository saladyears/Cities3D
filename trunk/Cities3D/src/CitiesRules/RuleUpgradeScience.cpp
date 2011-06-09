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
// Class: RuleUpgradeScience
// 
// Handles a science city upgrade.
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
class RuleUpgradeScience 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &)
	{
		RULE.Execute(shRulePlaySound, DataObject(SOUND_UPGRADE_SCIENCE));

		RULE.Execute(shRuleUpgradeCommon, DataObject(shScience));

		// Adjust stats.
		RULE.Execute(shRulePurchaseItem, DataObject(current(), shScience));
	}

	virtual void Unexecute(const DataObject &)
	{
	}

	virtual bool CanUndo() const
	{
		return true;
	}
};

IMPLEMENT_RULE(RuleUpgradeScience, CITIES)
