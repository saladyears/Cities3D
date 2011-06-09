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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkRuleRestartNetworkRestart
// 
// Mixes into <RuleRestartGame>.  Restarts the game from the <Game> State of 
// NetworkRestart.
//
// Derived From:
//     <Rule>
//
// Project:
//     <NetworkRules>
//
// RuleSet:
//     <Network>
//
// Mixin To:
//     <RuleRestartGame>
//
// Rules Called:
//     <NetworkRuleTakeSeat>
//
// Transmits To:
//     eventMessageUI
//     eventNetworkRestart
//
class NetworkRuleRestartNetworkRestart : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		Controller::get().Transmit(shEventMessageUI, 
			DataObject(stWaitingForPlayersToRejoin, GetGame()));

		// Fire up the restart dialog.
		Controller::get().Transmit(shEventNetworkRestart, true);
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(NetworkRuleRestartNetworkRestart, NetworkRestart, 
						   RuleRestartGame, NETWORK)
