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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkRuleRequestAddPlayer
// 
// Requests that the server add the player to the game with the given color.
// If this is the server, and the color is free, the request is granted.
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
//     <NetworkRuleAddPlayer>
//
class NetworkRuleRequestAddPlayer : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		// Only do stuff is this is the server and if we are still in the New
		// game state.
		if( (true == NET.Players().IsHost()) &&
			(shNew == gameData<HashString>(shState))
			)
		{
			const Player &player = object.read<Player>();
			const ColorType color = object.read<ColorType>(1);

			// The reason we scan the game like this is that due to network
			// latency, two players may request the same color at the same
			// time before either of them gets updated with who has that color.
			// This rule protects the game by only allowing the request to go
			// through if the color is free.
			bool safe = true;

			for(wxInt32 i = 0; i < numPlayers(); ++i)
			{
				const PlayerGame &playergame = this->playerGame(i);
				const Player &indexPlayer = playergame.player();

				// If we find the color being used or, heaven forbid, the
				// player already in the game, forbid the assignment.
				if( (color == playergame.color()) ||
					(player == indexPlayer))
				{
					safe = false;
					break;
				}
			}

			// If we made it through safely, assign the color.
			if(true == safe)
			{
				// Since we are calling this from within another rule, the
				// RuleEngine isn't going to send it on to the network.
				// Therefore, we have to do it manually.
				wxASSERT(NET.IsConnected());

				NET.SendRule(shNetworkRuleAddPlayer, object);
			}
		}
	}
};

IMPLEMENT_RULE(NetworkRuleRequestAddPlayer, NETWORK)
