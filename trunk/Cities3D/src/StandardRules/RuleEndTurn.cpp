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
// Class: RuleEndTurn
// 
// Handles ending a player's turn.
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
// Mixin To:
//     <>
//
// Rules Called:
//     <RuleUpdateNetworkUI>
//     <RuleIncrementTurnsPlayed>
//     <RuleIncrementPlayer>
//     <RuleAutoSave>
//
// Transmits To:
//     eventPlayerUI
//
class RuleEndTurn 
	: public Rule
	, public RuleExtensionMixin
{
public:
	virtual void Execute(const DataObject &object)
	{
		// Execute all extensions.
		ExecuteExtensions(object);

		// No more free stuff.
		playerGameData<wxInt32>(shFreeCities) = 0;

		// Clear the free road flag.
		erasePlayerGameData<wxInt32>(shRoadBuiltFree);
		erasePlayerGameData<wxInt32>(shCityBuiltFree);

		// Send out a system message.
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stEndsTheTurn));

		// Increment the number of game turns we've played.
		wxInt32& turn = gameData<wxInt32>(shTurn);
		turn++;

		wxLogDebug(wxT("Turn %d."), turn);

		// Every end of a turn counts as a turn.
		RULE.Execute(shRuleIncrementTurnsPlayed, DataObject());

		// Increment the current player.
		RULE.Execute(shRuleIncrementPlayer, DataObject());

		// Shut down all modeless dialog boxes.
		Controller::get().Transmit(shEventShutdownModelessUI, 0);

		Controller::get().Transmit(shEventPlayerUI, DataObject(GetGame(), -2));

		RULE.Execute(shRuleAutoSave, DataObject());

		// Play the sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_END_TURN));

		// See if they won on someone else's turn.
		wxInt32 player = current();
		DataObject input(player), output;
		RULE.Decide(shLogicWin, input, output);

		if(true == output.read<bool>())
		{
			// We have a winner!
			RULE.SetWinner(player);
		}
		// Handle any signaling that needs to be done.
		else
		{
			RULE.Execute(shRuleSignalTurn, DataObject());
		}

		// Trust the rule chain.
	}
};

IMPLEMENT_RULE(RuleEndTurn, STANDARD)
