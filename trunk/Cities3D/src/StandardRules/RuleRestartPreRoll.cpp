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
// Class: RuleRestartPreRoll
// 
// Restarts the game from the PreRoll state.  The player must decide
// whether to play a card, execute a build action, or roll the dice.
//
// Derived From:
//     <Rule>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// Mixin To:
//     <RuleRestartGame>
//
class RuleRestartPreRoll : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
        // The current player is now in a blocking action.
        RULE.Execute(shRuleBeginBlockingAction, DataObject(current()));

		// Since we enter this rule after we play a card pre-roll, we need
		// see if we can still play another card, or if it's time to roll
		// the dice.
		DataObject input(0), output;
		RULE.Decide(shLogicHasPreRollCard, input, output);
		bool hasCard = output.read<bool>();

		output.reset();
		RULE.Decide(shLogicHasPreRollAction, input, output);
		bool hasAction = output.read<bool>();

		// Set things up.
		if( (true == hasCard) ||
			(true == hasAction))
		{
			// Show or hide BuildUI.
			Controller::get().Transmit(shEventBuildUI, 
				DataObject(hasAction, GetGame()));

			// Show or hide ControlsUI.
			Controller::get().Transmit(shEventControlsUI, DataObject(false));

			wxString str, str2;

			// Update the text.
			if( (true == hasCard) &&
				(true == hasAction))
			{
				str = stPlayACardSelectAnAction;
				str2 = stWaitingPlayACardSelectAnAction;
			}
			else if(true == hasCard)
			{
				str = stPlayACardOrRoll;
				str2 = stWaitingPlayACardOrRoll;
			}
			else
			{
				str = stSelectAnActionOrRoll;
				str2 = stWaitingSelectAnActionOrRoll;
			}

			RULE.Execute(shRuleUpdateMessageUI, DataObject(str, str2));
		}
		else
		{
			RULE.Execute(shRuleRollDice, DataObject());
		}

		// Force an update of any player UI.
		Controller::get().Transmit(shEventPlayerUI, DataObject(GetGame(), -1));

		// Put things in wacky mode.
		Controller::get().Transmit(shEventPreRollCard, hasCard || hasAction);
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RuleRestartPreRoll, PreRoll, 
						   RuleRestartGame, STANDARD)
