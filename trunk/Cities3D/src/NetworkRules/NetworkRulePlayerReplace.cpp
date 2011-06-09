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
#include "RuleExtensionMixin.h"
#include "RuleSetDatabase.h"
#include "Controller.h"
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
// Class: NetworkRulePlayerReplace
// 
// Assigns the given spectator to the given color in the game.
//
// Derived From:
//     <Rule>
//     <RuleExtensionMixin>
//
// Project:
//     <NetworkRules>
//
// RuleSet:
//     <Network>
//
// Rules Called:
//     <NetworkRuleRemoveSpectator>
//
// Transmits To:
//     eventNewPlayer
//     eventNetworkRestartPlayers
//
class NetworkRulePlayerReplace 
	: public Rule
	, public RuleExtensionMixin
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(3 <= object.numItems());

		const Player &player = object.read<Player>();
		const wxInt32 index = object.read<wxInt32>(1);
		const wxString &str = object.read<wxString>(2);

		// Remove them from the spectator list.
		RULE.Execute(shNetworkRuleRemoveSpectator, object);

		// Add them to the game.
		playerGame(index).newPlayer(player);

		// The player has now joined, so clear any drop/quit
		// flag they had.
		playerGameData<HashString>(shNetworkDropFlag, index) = shEmpty;

		// Send out the update string.
		RULE.Execute(shRuleGenerateSystemMessage, DataObject(str));

		// Refresh the PlayerUI with the new name.
		Controller::get().Transmit(shEventNewPlayer, 
			DataObject(GetGame(), index));
		Controller::get().Transmit(shEventNetworkRestartPlayers, GetGame());

		// Allow any other processing of this event.
		ExecuteExtensions(object);
	}
};

IMPLEMENT_RULE(NetworkRulePlayerReplace, NETWORK)
