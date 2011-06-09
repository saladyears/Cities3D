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
// Class: RuleRollDiceCities
// 
// Extends the <RuleRollDice> rule to add the third die used in Cities &
// Knights.
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
// Transmits To:
//     eventThirdDie
//
class RuleRollDiceCities : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// Roll one six sided die.
		size_t die = RAND.pooled(6) + 1;

		// Transmit the roll.
		Controller::get().Transmit(shEventThirdDie, die);

		// Make a note.
		gameData<wxInt32>(shDie3) = die;
	}
};

IMPLEMENT_RULE_MIXIN(RuleRollDiceCities, RuleRollDice, CITIES)
