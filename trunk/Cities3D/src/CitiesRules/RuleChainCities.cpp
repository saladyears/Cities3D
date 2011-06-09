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
#include "PlayerData.h"
#include "RuleSetDatabase.h"
#include "LogicBranchDefault.h"
#include "Controller.h"
#include "GameEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicThirdDie
// 
// Determines what to do with the third die roll.  If it is a barbarian roll,
// handle it.  Otherwise, handle the city color roll.
//
// Derived From:
//     <Logic>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
class LogicThirdDie : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		wxInt32 roll = gameData<wxInt32>(shDie3);

		HashString rule;

		if(3 >= roll)
		{
			DataObject input(0), output;
			RULE.Decide(shLogicBarbarianAdvance, input, output);

			// If the barbarians are not allowed to advance (i.e. first two
			// turns), just continue on with the roll.
			if(true == output.read<bool>())
			{
				rule = shRuleBarbarianAdvance;
			}
			else
			{
				rule = shRuleMarkerPostDiceRoll;
			}
		}
		else
		{
			rule = shRuleProgressCards;
		}

		output = DataObject(rule);
	}
};

IMPLEMENT_LOGIC(LogicThirdDie, CITIES)

//---------------------------------------------------------------------------//
// Class: LogicDiscard
// 
// Determines if any progress cards must be discarded.
//
// Derived From:
//     <Logic>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
class LogicDiscard : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		// Run the discard logic.
		gameData<HashString>(shState) = shDiscardProgressCards;
		output = DataObject(shRuleRestartDiscardProgressCards);
	}
};

IMPLEMENT_LOGIC(LogicDiscard, CITIES)

//---------------------------------------------------------------------------//
// Class: LogicResetBarbarians
// 
// Determines if the barbarians need to be reset.  If they do, run the 
// barbarian reset rule.  Otherwise, continue post dice roll.
//
// Derived From:
//     <Logic>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
class LogicResetBarbarians : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		wxInt32 advance = gameData<wxInt32>(shBarbarianAdvance);

		HashString rule = shRuleMarkerPostDiceRoll;

		if(7 <= advance)
		{
			rule = shRuleResetBarbarians;
		}

		output = DataObject(rule);
	}
};

IMPLEMENT_LOGIC(LogicResetBarbarians, CITIES)

//---------------------------------------------------------------------------//
// Class: LogicAqueductBranch
// 
// Runs the rule to see if any players have aqueducts and get to use them.
//
// Derived From:
//     <Logic>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
class LogicAqueductBranch : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		gameData<HashString>(shState) = shAqueduct;

		HashString rule = shRuleAqueduct;
		output = DataObject(rule);
	}
};

//---------------------------- BRANCH LOGIC     -----------------------------//
//the cities rule chain
IMPLEMENT_DEFAULT_BRANCH(CITIES, RuleMarkerMiddleInitial, 
						 RuleContinueInitialCities);
IMPLEMENT_DEFAULT_BRANCH(CITIES, RuleContinueInitialCities, 
						 RuleRequestInitialPlacement);

// All third die related fallout must happen before any resources are gathered.
IMPLEMENT_PRIORITY_BRANCH(CITIES, RuleMarkerPostDiceRoll, LogicThirdDie, 150);

// After each roll, we have to check to see if any player must discard excess
// progress cards.
IMPLEMENT_PRIORITY_BRANCH(CITIES, RuleMarkerPostDiceRoll, LogicDiscard, 145);

// The barbarians must be reset after the third die roll if needed.
IMPLEMENT_PRIORITY_BRANCH(CITIES, RuleMarkerPostDiceRoll, 
						  LogicResetBarbarians, 140);

// After resources are gathered, see if anyone has the aqueduct and did not
// receive anything.  This is also set to occur after any gold is used in
// Seafarers, which is at priority 50.
IMPLEMENT_PRIORITY_BRANCH(CITIES, RuleMarkerPostDiceRoll, LogicAqueductBranch, 
						  25);
