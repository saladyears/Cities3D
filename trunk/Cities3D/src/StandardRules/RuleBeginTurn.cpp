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
#include "RuleExtensionMixin.h"
#include "RuleSetDatabase.h"
#include "Controller.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleBeginTurn
// 
// Does stuff.
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
// Rules Called:
//     <RuleRestartTurn>
//
// Transmits To:
//     eventControlsUI
//
class RuleBeginTurn 
	: public Rule
	, public RuleExtensionMixin
{
public:
	virtual void Execute(const DataObject &)
	{
		gameData<HashString>(shState) = shTurn;

		// Set turn stuff.
		RULE.Execute(shRuleRestartTurn, DataObject());

		// Clean up rolls.
		eraseGameData<HashString>(shDie1);
		eraseGameData<HashString>(shDie2);

		// Allow other extensions.
		ExecuteExtensions(DataObject());
	}
};

IMPLEMENT_RULE(RuleBeginTurn, STANDARD)
