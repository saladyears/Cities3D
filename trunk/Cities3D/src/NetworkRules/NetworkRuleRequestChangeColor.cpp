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
// Class: NetworkRuleRequestChangeColor
// 
// Requests that the server change the given player to the given color.  If
// this is the server, and the color is free, the request is granted.
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
//     <RuleUIChangeColor>
//
class NetworkRuleRequestChangeColor : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		// Only do stuff is this player is the server.
		if(true == NET.Players().IsHost())
		{
			const Player &player = object.read<Player>();
			const ColorType color = object.read<ColorType>(1);

			// They are only allowed to change colors themself if the color 
			// they want is open, so scan the current players to see if that
			// color is available.
			wxInt32 playerIndex = -1;
			bool safe = true;
			for(wxInt32 i = 0; i < numPlayers(); ++i)
			{
				const PlayerGame &playergame = this->playerGame(i);
				const Player &indexPlayer = playergame.player();

				// If we find this player, keep track of their index so we
				// can change their color later without reiterating.
				if(indexPlayer == player)
				{
					playerIndex = i;
				}

				// If the color is being used, they can't switch.
				if(playergame.color() == color)
				{
					safe = false;
					break;
				}
			}

			// Switch colors if possible.
			if( (true == safe) &&
				(-1 != playerIndex)
				)
			{
				// Since we are calling this from within another rule, the
				// RuleEngine isn't going to send it on to the network.
				// Therefore, we have to do it manually.
				wxASSERT(NET.IsConnected());

				NET.SendRule(shNetworkRuleChangeColor, object);
			}
		}
	}
};

IMPLEMENT_RULE(NetworkRuleRequestChangeColor, NETWORK)
