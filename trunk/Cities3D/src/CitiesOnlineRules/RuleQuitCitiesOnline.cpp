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
// Class: RuleQuitCitiesOnline
// 
// Extension to NetworkRuleQuit to report a client quitting the game.
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
//     <NetworkRuleQuit>
//
class RuleQuitCitiesOnline : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		const Player &player = object.read<Player>();

		// Only report the quit if we are the host, or the
		// quitting player, and we're in a Cities Online game.
		if( (TRUE == gameData<wxInt32>(shCitiesOnline)) &&
			((true == NET.Players().IsHost()) ||
			(NET.Players().GetThisPlayer() == player)))
		{
            DataObject input(-1), output;
            RULE.Decide(shLogicIsThisPlayer, input, output);
            wxInt32 actualIndex = output.read<wxInt32>(1);

			// And, the game id.
			wxInt32 id = gameData<wxInt32>(shID);
            wxInt32 passive = playerGameData<wxInt32>(shPassiveQuit, actualIndex);

			HashString command = HashString::Format(shQuitCmd.c_str(),
				id, player.Id(), passive);
			Controller::get().Transmit(shEventSynchronize, command);

			// Additionally, if it is this player, signal Cities Online that
			// we are not in a game.
			if(NET.Players().GetThisPlayer() == player)
			{
				Controller::get().Transmit(shEventSynchronize, shMinus);
			}
		}
	}
};

IMPLEMENT_RULE_MIXIN(RuleQuitCitiesOnline, NetworkRuleQuit, CITIESONLINE)
