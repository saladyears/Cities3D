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
#include "Map.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleStartHostCommandGame
// 
// Starts a game in response to a host command.
//
// Derived From:
//     <Rule>
//
// Project:
//     <CitiesOnlineRules>
//
// RuleSet:
//     <CitiesOnline>
//
//
class RuleStartHostCommandGame : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		const Player& hostPlayer = object.read<Player>();

		// Randomize players.
		RULE.Execute(shRuleRandomizePlayers, DataObject(-1));

		// Initialize the board.
		const Map::StringPairArray& rulesets = MAP.rulesets();
		Controller::get().Transmit(shEventLoadRulesets, rulesets);
		Controller::get().Transmit(shEventStartGame, true);

		// Start the game.
		RULE.Execute(shRule0, DataObject());

		// Since we'll be restarting using the network code, we need
		// to set the restart state properly.
		const HashString& state = gameData<HashString>(shState);
		gameData<HashString>(shNetworkRestartState) = state;

		Controller::get().Transmit(shEventMessageUI, 
			DataObject(stWaitingForPlayersToJoin, GetGame()));

		// Fire up the restart dialog.  The false signifies that we
		// we'll use the start game text instead of restart.
		Controller::get().Transmit(shEventNetworkRestart, false);

		// Immediately check to see if the host takes any of the seats
		// in the game.
		RULE.Execute(shNetworkRuleTakeSeat, DataObject(hostPlayer, false));
	}
};

IMPLEMENT_RULE(RuleStartHostCommandGame, CITIESONLINE)
