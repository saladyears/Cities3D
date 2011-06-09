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
#include "NetworkPlayers.h"
#include "INetworkEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkRuleQuit
// 
// Marks a player as quitting the game.
//
// Derived From:
//     <RuleExtensionMixin>
//
// Project:
//     <NetworkRules>
//
// RuleSet:
//     <Network>
//
class NetworkRuleQuit 
	: public Rule
	, public RuleExtensionMixin
{
public:
	virtual void Execute(const DataObject &object)
	{
		// Execute extensions for any other plugins that want to know.
		ExecuteExtensions(object);

		const Player &player = object.read<Player>();

		// Find this player and mark them as quitting, if they are in game.
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			if(playerGame(i).player() == player)
			{
				playerGameData<HashString>(shNetworkDropFlag, i) = shQuit;
				break;
			}
		}
		
		// If this player is also the host, mark them more directly.
		if(player == NET.Players().GetHostPlayer())
		{
			gameData<wxInt32>(shHostQuit) = TRUE;
		}
	}
};

IMPLEMENT_RULE(NetworkRuleQuit, NETWORK)
