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
// Class: RuleRestartTurnMasterMerchantSteal
// 
// Restarts the game from a player choosing cards to steal after playing a 
// master merchant card and selecting a victim.
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
//     <RuleRestartGame>
//
class RuleRestartTurnMasterMerchantSteal : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
        // The current player is now in a blocking action.
        RULE.Execute(shRuleBeginBlockingAction, DataObject(current()));

		wxInt32 index = gameData<wxInt32>(shMasterMerchantVictim);
		const wxString& name = playerGame(index).player().Name();

		wxString str = wxString::Format(
			stSelectTwoCardsToStealFrom.c_str(), swStringFormat.c_str(), 
			name.c_str());
		wxString str2 = wxString::Format(
			stWaitingSelectTwoCardsToSteal.c_str(), 
            swStringFormat.c_str(), name.c_str());
		RULE.Execute(shRuleUpdateMessageUI, DataObject(str, str2));

		Controller::get().Transmit(shEventControlsUI, DataObject(false));
		Controller::get().Transmit(shEventBuildUI, 
				DataObject(false, GetGame()));

		Controller::get().Transmit(shEventMasterMerchantResource, GetGame());
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RuleRestartTurnMasterMerchantSteal, 
						   TurnMasterMerchantSteal, RuleRestartGame, CITIES)
