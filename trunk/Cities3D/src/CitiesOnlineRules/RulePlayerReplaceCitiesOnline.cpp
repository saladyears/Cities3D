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
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePlayerReplaceCitiesOnline
// 
// Extension to NetworkRulePlayerReplace to report a client connecting to the
// host.
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
//     <NetworkRulePlayerReplace>
//
class RulePlayerReplaceCitiesOnline : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());
		const Player &player = object.read<Player>();

		// Only report the connection if we are the host, or the
		// connecting player, and we're in a Cities Online game.
		if( (TRUE == gameData<wxInt32>(shCitiesOnline)) &&
			((true == NET.Players().IsHost()) ||
			(NET.Players().GetThisPlayer() == player)))
		{
			// Get the player's id.
			wxInt32 playerId = player.Id();

			// And, the game id.
			wxInt32 id = gameData<wxInt32>(shID);

			// Get the player's actual index.
			wxInt32 actualIndex = 0;
			for(wxInt32 i = 0; i < numPlayers(); ++i)
			{
				const Player &currentPlayer = playerGame(i).player();
				if(currentPlayer == player)
				{
					actualIndex = i;
					break;
				}
			}

			ColorType type = playerGame(actualIndex).color();
			HashString color = HashString(ColorInfoData::InternalName(type));

			HashString command = HashString::Format(shConnectCmd.c_str(), id, playerId, 
                actualIndex + 1, color.c_str());
			Controller::get().Transmit(shEventSynchronize, command);

			// Additionally, if it is this player, signal Cities Online that
			// we are now in a game.
			if(NET.Players().GetThisPlayer() == player)
			{
				Controller::get().Transmit(shEventSynchronize, shPlus);
			}
		}
	}
};

IMPLEMENT_RULE_MIXIN(RulePlayerReplaceCitiesOnline, 
					 NetworkRulePlayerReplace, CITIESONLINE)
