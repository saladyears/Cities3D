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
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkRuleConnectionLostInGame
// 
// Handles the loss of a player in game.
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
//
class NetworkRuleConnectionLostInGame 
	: public Rule
	, public RuleExtensionMixin
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxLogDebug(wxT("NetworkRuleConnectionLostInGame: Entry."));

		// Execute any extensions that want to handle this.
		ExecuteExtensions(object);

		const Player &player = object.read<Player>();

		// No matter what the case, the player needs to be replaced with the
		// NullPlayer in the current game so their seat becomes open.
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			if(player == playerGame(i).player())
			{
				// Set them as Null.
				playerGame(i).newPlayer(NullPlayer);

				// Refresh the PlayerUI with the new name.
				Controller::get().Transmit(shEventNewPlayer, 
					DataObject(GetGame(), i));
				break;
			}
		}

		// If we are in the game right now we need to shut everything
		// down and fire up the Network Restart dialog.
		if(shNetworkRestart != gameData<HashString>(shState))
		{
			Controller::get().Transmit(shEventShutdownUI, 0);

			gameData<HashString>(shNetworkRestartState) = 
				gameData<HashString>(shState);
			gameData<HashString>(shState) = shNetworkRestart;
			wxLogDebug(wxT("NetworkRuleConnectionLostInGame: State is now ")
				wxT("NetworkRestart."));

			RULE.Execute(shRuleRestartGame, DataObject());
		}

		// Always update the players in the network restart list.
		Controller::get().Transmit(shEventNetworkRestartPlayers, GetGame());
		Controller::get().Transmit(shEventSpectator, 0);

		wxLogDebug(wxT("NetworkRuleConnectionLostInGame: Exit."));
	}
};

IMPLEMENT_RULE(NetworkRuleConnectionLostInGame, NETWORK)
