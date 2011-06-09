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
// Class: NetworkRuleAllSeatsFull
// 
// Restarts the network game when all seats have been filled.
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
//     <RuleRestartGame>
//
// Transmits To:
//     eventNetworkAllSeatsFull
//
class NetworkRuleAllSeatsFull : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// Shut down the rejoin dialog.
		Controller::get().Transmit(shEventNetworkAllSeatsFull, 0);

		// Set the state to the last state before restart.
		gameData<HashString>(shState) = 
			gameData<HashString>(shNetworkRestartState);
		eraseGameData<HashString>(shNetworkRestartState);

		// Reset everyone's play countdown timers.
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			playerGameData<wxTimeSpan>(shRestartTime, i) = 
				wxTimeSpan(0, 0, 0, -1);
		}

		// Restart.
		RULE.Execute(shRuleRestartGame, DataObject());
	}
};

IMPLEMENT_RULE(NetworkRuleAllSeatsFull, NETWORK)
