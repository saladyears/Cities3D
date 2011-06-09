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
// Class: NetworkRuleStartGame
// 
// Births a network game into existence.
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
//     <RuleRandomizePlayers>
//     <Rule0>
//
// Game State:
//     NetworkLaunch
//
// Transmits To:
//     eventStartNetworkGame
//
class NetworkRuleStartGame : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		wxString players;

		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			if(0 != i)
			{
				players += swCommaSpace;
			}
			players += playerGame(i).player().Name();
		}

		wxLogDebug(wxT("Starting a new network game with %s."), 
			players.c_str());

		// Set the state different than New so anyone joining in the brief 
		// time between when the host will not be able to add themself as a
		// player to the game.
		gameData<HashString>(shState) = shNetworkLaunch;

		// Randomize the players.  This works because every client is using
		// the same pool of random numbers.
		RULE.Execute(shRuleRandomizePlayers, DataObject());

		// Start the game.
		const Map::StringPairArray& rulesets = MAP.rulesets();
		Controller::get().Transmit(shEventLoadRulesets, rulesets);
		Controller::get().Transmit(shEventStartGame, true);

		// Start with the first rule.
		RULE.Execute(shRule0, DataObject());
	}
};

IMPLEMENT_RULE(NetworkRuleStartGame, NETWORK)
