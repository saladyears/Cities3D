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
// Class: RulePlayCardYearOfPlenty
// 
// Plays a year of plenty card.
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
class RulePlayCardYearOfPlenty : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		// Update common card data.
		RULE.Execute(shRulePlayCardDevCards, object);

		// Play the sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_PLAY_YOP_CARD));

		// They have also now played a card this turn.
		playerGameData<wxInt32>(shPlayedCard)++;

		Controller::get().Transmit(shEventPlayerUI, DataObject(GetGame(), 
			current()));

		// Since we are going into a dialog window that they cannot leave from,
		// set the game state so that we can restart properly if the game
		// stops.
		gameData<HashString>(shState) = shTurnYearOfPlenty;

		// Just use the restart logic to fire up the dialog.
		RULE.Execute(shRuleRestartTurnYearOfPlenty, DataObject());
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RulePlayCardYearOfPlenty, YearOfPlenty, 
						   RulePlayCard, DEVCARDS)
