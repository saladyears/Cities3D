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
#include "IRuleEngine.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicPostRoll
// 
// Determines what to do after dice are rolled.  If the dice total equals 
// seven, then players may potentially lose resources.  Otherwise, resources
// are gathered.
//
// Derived From:
//     <Logic>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
class LogicPostRoll : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		wxInt32 die1 = gameData<wxInt32>(shDie1);
		wxInt32 die2 = gameData<wxInt32>(shDie2);
		wxInt32 roll = die1 + die2;

		HashString rule;

		// Transmit the roll to the UI.
		Controller::get().Transmit(shEventDiceRoll, DataObject(die1, die2));

		if(7 == roll)
		{
			rule = shRuleHandle7;
		}
		else
		{
			rule = shRuleGatherResources;
		}

		output = DataObject(rule);
	}
};

IMPLEMENT_LOGIC(LogicPostRoll, STANDARD)

//---------------------------------------------------------------------------//
// Class: LogicBeginTurn
// 
// Sends the game into the turn beginning.
//
// Derived From:
//     <Logic>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
class LogicBeginTurn : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(shRuleBeginTurn);
	}
};

IMPLEMENT_LOGIC(LogicBeginTurn, STANDARD)

//---------------------------------------------------------------------------//
// Class: LogicNextTurn
// 
// Starts the next turn.
//
// Derived From:
//     <Logic>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// LogicCalled
//
class LogicNextTurn : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		HashString rule = shRuleNothing;

		// Only do stuff if someone has won the game.
		if(-1 == RULE.GetWinner())
		{
			// Check to see if the player has any pre roll abilities.
			DataObject input(0);
			RULE.Decide(shLogicHasPreRollCard, input, output);
			bool hasCard = output.read<bool>();

			output.reset();
			RULE.Decide(shLogicHasPreRollAction, input, output);
			bool hasAction = output.read<bool>();

			if( (true == hasCard) ||
				(true == hasAction))
			{
				gameData<HashString>(shState) = shPreRoll;
				rule = shRuleRestartPreRoll;
			}
			else
			{
				rule = shRuleRollDice;
			}
		}

		output = DataObject(rule);
	}
};

IMPLEMENT_LOGIC(LogicNextTurn, STANDARD)

//---------------------------- BRANCH LOGIC     -----------------------------//
//the standard rule chain

//initial placements
IMPLEMENT_DEFAULT_BRANCH(STANDARD, RuleMarkerStartInitial, 
						 RuleStartInitialPlacements);
IMPLEMENT_DEFAULT_BRANCH(STANDARD, RuleStartInitialPlacements, 
						 RuleRequestInitialPlacement);
IMPLEMENT_DEFAULT_BRANCH(STANDARD, RulePlaceInitialPlacement, 
						 RuleMarkerInitialRoad);
IMPLEMENT_DEFAULT_BRANCH(STANDARD, RuleMarkerInitialRoad, 
						 RuleRequestInitialRoad);
IMPLEMENT_DEFAULT_BRANCH(STANDARD, RuleMarkerMiddleInitial, 
						 RuleContinueInitialPlacements);
IMPLEMENT_DEFAULT_BRANCH(STANDARD, RuleContinueInitialPlacements, 
						 RuleRequestInitialPlacement);

// After initial placements, roll the dice.
IMPLEMENT_DEFAULT_BRANCH(STANDARD, RuleMarkerEndInitial, RuleRollDice)

// After rolling the dice, start handling post-roll fallout.
IMPLEMENT_DEFAULT_BRANCH(STANDARD, RuleRollDice, RuleMarkerPostDiceRoll)

// Dice roll branches.
IMPLEMENT_PRIORITY_BRANCH(STANDARD, RuleMarkerPostDiceRoll, LogicPostRoll, 
						  100);
IMPLEMENT_BRANCH(STANDARD, RuleMarkerPostDiceRoll, LogicBeginTurn);

// At the end of the turn handle the turn cleanup.
IMPLEMENT_DEFAULT_BRANCH(STANDARD, RuleMarkerEndTurn, RuleEndTurn);

// After the turn succesfully ends, start the next turn.
IMPLEMENT_BRANCH(STANDARD, RuleEndTurn, LogicNextTurn);
