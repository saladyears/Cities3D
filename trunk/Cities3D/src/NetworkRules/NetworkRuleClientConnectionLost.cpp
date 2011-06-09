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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkRuleClientConnectionLost
// 
// Handles a lost connection from the client's perspective.  This occurs when
// either the server shuts down or drops connection.
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
// Transmits To:
//     eventGameConnect,
//     eventMessageBox
//
class NetworkRuleClientConnectionLost 
	: public Rule
	, public RuleExtensionMixin
{
public:
	virtual void Execute(const DataObject &object)
	{
		// Allow any extensions to process the lost connection.
		ExecuteExtensions(object);

		if(shNew == gameData<HashString>(shState))
		{
			// Alert the UI about the disconnect.
			Controller::get().Transmit(shEventGameConnect, DataObject(false));
		}
		else
		{
			// Shutdown everything.
			Controller::get().Transmit(shEventShutdownUI, 0);
			Controller::get().Transmit(shEventMessageUI, 
				DataObject(stDisconnectedFromServer, GetGame()));
		}

		wxString str;

		if(TRUE == gameData<wxInt32>(shHostQuit))
		{
			const Player &hostPlayer = NET.Players().GetHostPlayer();
			str = wxString::Format(stShutdownTheServer.c_str(),
                hostPlayer.Name().c_str());
		}
		else
		{
			str = stLostConnectionToTheServer;
		}

		Controller::get().Transmit(shEventMessageBox, str);
	}
};

IMPLEMENT_RULE(NetworkRuleClientConnectionLost, NETWORK)
