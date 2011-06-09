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
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleWin
// 
// Ends the game when a player wins.
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
// Game State:
//     Complete
//
// Transmits To:
//     eventPlayerUI,
//     eventShutdownUI
//
class RuleWin
	: public Rule
	, public RuleExtensionMixin
{
public:
	virtual void Execute(const DataObject &object)
	{
		// Play the sound.
		DataObject input(-1), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);

		wxString sound;
		if(true == output.read<bool>())
		{
			sound = SOUND_WIN_GAME;
		}
		else
		{
			sound = SOUND_LOSE_GAME;
		}

		RULE.Execute(shRulePlaySound, DataObject(sound));

		// Execute any extensions to winning.
		ExecuteExtensions(object);

		// Set the final state properly.
		gameData<HashString>(shState) = shComplete;

		// Clear all flags.
		RULE.Execute(shRuleResetTileFlags, DataObject());
		RULE.Execute(shRuleResetObjectFlags, DataObject());

		// Update PlayerUI one final time.
		Controller::get().Transmit(shEventPlayerUI, DataObject(GetGame(), -3));

		// Shut everything down.
		Controller::get().Transmit(shEventShutdownUI, 0);

		// Set the final text.
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stWinsTheGame));
		RULE.Execute(shRuleUpdateMessageUI, DataObject(stHasWonTheGame, 
            stHasWonTheGame));

		// Finally, save the game to the complete game database.
		GameEngine::get().SaveGame();
	}
};

IMPLEMENT_RULE(RuleWin, STANDARD)
