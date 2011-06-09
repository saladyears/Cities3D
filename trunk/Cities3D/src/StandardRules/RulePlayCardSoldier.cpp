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
// Class: RulePlayCardSoldier
// 
// Plays a soldier card.
//
// Derived From:
//     <Rule>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <DevCards>
//
// Mixin To:
//     <RulePlayCard>
//
class RulePlayCardSoldier : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		// Update common card data.
		RULE.Execute(shRulePlayCardDevCards, object);
		
		// New state.
		gameData<HashString>(shState) = shRobber;

		// Play the sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_PLAY_SOLDIER_CARD));

		// Enter move the robber mode.
		RULE.Execute(shRuleRestartRobber, DataObject());

		// They have also now played a card this turn.
		playerGameData<wxInt32>(shPlayedCard)++;

		// A soldier also check for Largest Army, which is done last, in case
		// the player wins with the Largest Army points.
		RULE.Execute(shRuleLargestArmyCheck, DataObject());
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RulePlayCardSoldier, Soldier, 
						   RulePlayCard, DEVCARDS)
