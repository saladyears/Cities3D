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
#include "Controller.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRestartPostDiceRoll
// 
// Restarts the game from the PostDiceRoll state.  This state is only executed at
// the end of playing the robber post 7 roll.  The game is never actually in
// this state, it just allows the correct flow to happen since playing the 
// robber can happen at a variety of times in the game.
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
// Mixin To:
//     <RuleRestartGame>
//
class RuleRestartPostDiceRoll : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		RULE.Execute(shRuleMarkerPostDiceRoll, DataObject());
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RuleRestartPostDiceRoll, PostDiceRoll, 
						   RuleRestartGame, STANDARD)
