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
#include "RuleHashMixin.h"
#include "RuleSetDatabase.h"
#include "NetworkPlayers.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkRuleConnectionLost
// 
// Handles a player dropping from the network game.  Does a variety of things
// based on the Network state of the player who dropped.
//
// Derived From:
//     <Rule>,
//     <RuleHashMixin>
//
// Project:
//     <NetworkRuleConnectionLost>
//
// RuleSet:
//     <Network>
//
// Rules Called:
//     <NetworkRuleResetReady>
//
class NetworkRuleConnectionLost 
	: public Rule
	, public RuleHashMixin
{
public:
	virtual void Execute(const DataObject &object)
	{
		// Find the player first.
		const Player &player = object.read<Player>();

		// If they aren't in the game, they must be a spectator, which has
		// less stringent drop tracking.
		HashString state = shSpectator;
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			if(playerGame(i).player() == player)
			{
				state = playerGame(i).read<HashString>(shNetworkDropFlag);
				break;
			}
		}

		// If there's no state set, it's a drop.
		if(true == state.empty())
		{
			state = shDrop;
		}

		// Execute based on what the value is set to.
		if(false == ExecuteHash(state, object))
		{
			wxLogError(wxT("Programmer Error: No State %s in ")
				wxT("NetworkRuleConnectionLost"), state.cwx_str());
		}

		// Reset everyone's ready state if they weren't a spectator and the
		// game isn't ongoing.
		if( (shSpectator != state) &&
			(shNew == gameData<HashString>(shState)))
		{
			RULE.Execute(shNetworkRuleResetReady, DataObject());
		}
	}
};

IMPLEMENT_RULE(NetworkRuleConnectionLost, NETWORK)
