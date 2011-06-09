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
#include "NetworkPlayers.h"
#include "INetworkEngine.h"
#include "Controller.h"
#include "Map.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkRuleJoinGame
// 
// Joins an ongoing network game.
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
// Rules Called:
//     <RulePrepareUI>
//     <RuleRestartGame>
//
// Transmits To:
//     eventStartNetworkGame
//
class NetworkRuleJoinGame : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// Start the game.
		const Map::StringPairArray& rulesets = MAP.rulesets();
		Controller::get().Transmit(shEventLoadRulesets, rulesets);
		Controller::get().Transmit(shEventStartGame, false);

		// We're basically acting as if we're restarting the current game.
		RULE.Execute(shRulePrepareUI, DataObject());
		RULE.Execute(shRuleRestartGame, DataObject());
	}
};

IMPLEMENT_RULE(NetworkRuleJoinGame, NETWORK)
