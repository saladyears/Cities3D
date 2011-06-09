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
#include "NetworkPlayers.h"
#include "INetworkEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkRuleResetReady
// 
// Resets the ready state of all players to not ready, except for the host.
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
// Transmits To:
//     eventPreGame
//
class NetworkRuleResetReady : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		const Player &hostPlayer = NET.Players().GetHostPlayer();

		// Reset every player's ready state to not ready.
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			const Player &thisPlayer = playerGame(i).player();
			if(thisPlayer != hostPlayer)
			{
				playerGameData<wxInt32>(shReady, i) = FALSE;
			}
		}

		Controller::get().Transmit(shEventPreGame, GetGame());
	}
};

IMPLEMENT_RULE(NetworkRuleResetReady, NETWORK)
