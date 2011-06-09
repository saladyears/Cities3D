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
// Class: RuleRestartTurn
// 
// Restarts the game from the Turn state.
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
//     <RuleRestartGame>
//
// Rules Called:
//     <RuleUpdateMessageUI>
//
// Game State:
//     Turn
//
// Transmits To:
//     eventControlsUI
//
class RuleRestartTurn 
    : public Rule
    , public RuleExtensionMixin
{
public:
	virtual void Execute(const DataObject &)
	{
		if(shTurn == gameData<HashString>(shState))
		{
			// Set MessageUI text.
			RULE.Execute(shRuleUpdateMessageUI, DataObject(stItIsYourTurn, 
                stWaitingFinishTurn));

			// Update the user actions.
			Controller::get().Transmit(shEventControlsUI, DataObject(true));

			// Update the BuildUI.
			Controller::get().Transmit(shEventBuildUI, 
					DataObject(true, GetGame()));

            // Allow other extensions.
		    ExecuteExtensions(DataObject());
		}
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RuleRestartTurn, Turn, RuleRestartGame, STANDARD)
