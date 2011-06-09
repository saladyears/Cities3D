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
#include "RNG.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRollDiceAlchemist
// 
// Extends the <RuleRollDice> rule to override a dice roll in the case of an
// Alchemist card.
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
//     <RuleRollDice>
//
class RuleRollDiceAlchemist : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		wxInt32 die1 = gameData<wxInt32>(shAlchemistDie1);
		wxInt32 die2 = gameData<wxInt32>(shAlchemistDie2);

		if( (0 != die1) &&
			(0 != die2))
		{
			gameData<wxInt32>(shDie1) = die1;
			gameData<wxInt32>(shDie2) = die2;
		}
	}
};

IMPLEMENT_RULE_MIXIN(RuleRollDiceAlchemist, RuleRollDice, CITIES)
