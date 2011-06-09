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
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkRuleRestartGame
// 
// Restarts a network game from a saved point on disk.  Saves the current game
// state and sets it to NetworkRestart, then calls RuleRestartGame for normal
// processing.
//
// Derived From:
//     <Rule>
//
// Project:
//     <CoreRules>
//
// RuleSet:
//     <Core>
//
// Rules Called:
//      <RuleRestartGame>
//
// Game State:
//      NetworkRestart
//
class NetworkRuleRestartGame : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		wxString players;

		// Empty all of the players in the game so they can be filled in as 
		// players rejoin.
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			if(0 != i)
			{
				players += swCommaSpace;
			}
			players += playerGame(i).player().Name();

			playerGame(i).newPlayer(NullPlayer);
		}

		wxLogDebug(wxT("Restarting a network game with %s."), 
			players.c_str());

		gameData<HashString>(shNetworkRestartState) = 
			gameData<HashString>(shState);
		gameData<HashString>(shState) = shNetworkRestart;

		RULE.Execute(shRulePrepareUI, DataObject());
		RULE.Execute(shRuleRestartGame, DataObject());

		// Immediately check to see if the host takes any of the seats
		// in the game.
		RULE.Execute(shNetworkRuleTakeSeat, 
			DataObject(NET.Players().GetThisPlayer()));
	}
};

IMPLEMENT_RULE(NetworkRuleRestartGame, NETWORK);
