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
#include "Controller.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkRuleRemovePlayer
// 
// Removes a player from the game and adds them to the spectator list.
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
//     <RuleUIRemovePlayer>
//     <NetworkRuleResetReady>
//     <RuleGenerateSystemMessage>
//
class NetworkRuleRemovePlayer : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		const Player &player = object.read<Player>();

		// First, make sure the player is even in the game.  It is possible
		// for a client to send this message after a host has already removed
		// them.
		bool inGame = false;
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			if(playerGame(i).player() == player)
			{
				inGame = true;
				break;
			}
		}

		if(true == inGame)
		{
			// Add them back to the spectator list.
			RULE.Execute(shNetworkRuleAddSpectator, object);
			RULE.Execute(shRuleUIRemovePlayer, object);

			const Player &player = object.read<Player>();
			wxString str = wxString::Format(stBecomesASpectator.c_str(),
				player.Name().c_str());
			RULE.Execute(shRuleGenerateSystemMessage, DataObject(str));

			// Reset the ready state of everyone still left in the game as they
			// might not want to play with this many players.
			RULE.Execute(shNetworkRuleResetReady, DataObject());
		}
	}
};

IMPLEMENT_RULE(NetworkRuleRemovePlayer, NETWORK)
