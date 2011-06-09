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
#include "INetworkEngine.h"
#include "NetworkPlayers.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleClientConnectionLostCitiesOnline
// 
// Extension to NetworkRuleClientConnectionLost to report a client losing
// their connection to the site.
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
class RuleClientConnectionLostCitiesOnline : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// Only report if we're in a Cities Online game and not a spectator.
		if( (TRUE == gameData<wxInt32>(shCitiesOnline)) &&
			(false == NET.Players().IsSpectator()))
		{
			// Get this player's id.
			wxInt32 playerId = NET.Players().GetThisPlayer().Id();

			// And, the game id.
			wxInt32 id = gameData<wxInt32>(shID);

			HashString command = HashString::Format(shDisconnectCmd.c_str(),
				id, playerId);
			Controller::get().Transmit(shEventSynchronize, command);

			// Additionally, signal Cities Online that we are not in a game.
			Controller::get().Transmit(shEventSynchronize, shMinus);
		}
	}
};

IMPLEMENT_RULE_MIXIN(RuleClientConnectionLostCitiesOnline, 
					 NetworkRuleClientConnectionLost, CITIESONLINE)
