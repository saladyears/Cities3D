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
// Class: RulePlayCardSmith
// 
// Plays a smith card.
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
// Mixin To:
//     <RulePlayCard>
//
class RulePlayCardSmith : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		const HashString& card = object.read<HashString>();
		
		RULE.Execute(shRulePlayProgressCardStart, DataObject(card, shScience));

		// Play the sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_PLAY_SMITH_CARD));

		// The Smith is very simple.  The player just gets two free knight
		// upgrades.
		playerGameData<wxInt32>(shFreeKnightUpgrades) += 2;

		RULE.Execute(shRulePlayProgressCardEnd, DataObject());
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RulePlayCardSmith, Smith, 
						   RulePlayCard, CITIES)
