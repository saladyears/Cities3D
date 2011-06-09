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
#include "RNG.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRollDice
// 
// Rolls the dice.
//
// Derived From:
//     <Rule>
//     <RuleExtensionMixin>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// Rules Called:
//     <RulePlaySound>
//
// Transmits To:
//     eventDiceRoll
//
class RuleRollDice
	: public Rule
	, public RuleExtensionMixin
{
public:
	virtual void Execute(const DataObject &object)
	{
		bool valid = false;
		size_t die1 = 0, die2 = 0;

		do
		{
			// Roll two six sided dice.
			die1 = RAND.get().pooled(6) + 1;
			die2 = RAND.get().pooled(6) + 1;

			DataObject input(die1, die2), output;
			RULE.Decide(shLogicIsValidDiceRoll, input, output);
			valid = output.read<bool>();
		}
		while(false == valid);

		// Make a note of these rolls so future rules can use them.
		gameData<wxInt32>(shDie1) = die1;
		gameData<wxInt32>(shDie2) = die2;

		// Execute any extensions to dice rolling.
		ExecuteExtensions(object);

		RULE.Execute(shRuleRollDiceText, DataObject());	
		RULE.Execute(shRulePlaySound, DataObject(SOUND_ROLL_DICE));

		wxLogDebug(wxT("Dice roll: %d"), (die1 + die2));
	}
};

IMPLEMENT_RULE(RuleRollDice, STANDARD)
