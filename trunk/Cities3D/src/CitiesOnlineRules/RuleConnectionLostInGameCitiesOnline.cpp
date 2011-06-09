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
#include "stdCommon.h"
#include "Controller.h"
#include "INetworkEngine.h"
#include "NetworkPlayers.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleConnectionLostInGameCitiesOnline
// 
// Extension to NetworkRuleConnectionLostInGame to report a client losing
// their connection to the site, from the host.
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
// Mixin To:
//     <NetworkRuleClientConnectionLost>
//
class RuleConnectionLostInGameCitiesOnline : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		const Player &player = object.read<Player>();
		bool quit = false;

		// Find the player and see if they quit.
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			if( (playerGame(i).player() == player) &&
				(shQuit == playerGameData<HashString>(shNetworkDropFlag, i)))
			{
				quit = true;
				break;
			}
		}
			
		// Only report if this is the host and we're in a Cities Online game,
		// and the player is not a spectator, and the player did not quit.  IF
		// they quit, we already took care of it.
		if( (TRUE == gameData<wxInt32>(shCitiesOnline)) &&
			(false == quit) &&
			(true == NET.Players().IsHost()) &&
			(false == NET.Players().IsSpectator(player)))
		{
			// Get this player's id.
			wxInt32 playerId = player.Id();

			// And, the game id.
			wxInt32 id = gameData<wxInt32>(shID);

			HashString command = HashString::Format(shDisconnectCmd.c_str(),
				id, playerId);
			Controller::get().Transmit(shEventSynchronize, command);
		}
	}
};

IMPLEMENT_RULE_MIXIN(RuleConnectionLostInGameCitiesOnline, 
					 NetworkRuleConnectionLostInGame, CITIESONLINE)
