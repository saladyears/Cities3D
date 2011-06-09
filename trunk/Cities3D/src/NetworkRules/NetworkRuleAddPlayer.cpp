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
#include "Controller.h"
#include "ColorInfoData.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkRuleAddPlayer
// 
// Adds a player to the game and removes them from the spectator list.
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
//     <NetworkRuleRemoveSpectator>
//     <RuleUIAddPlayer>
//     <NetworkRuleResetReady>
//
// Transmits To:
//     eventPreGame
//
class NetworkRuleAddPlayer : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxBOOL ready = object.read<wxInt32>(2);

		RULE.Execute(shNetworkRuleRemoveSpectator, object);
		RULE.Execute(shRuleAddPlayer, object);

		// Set their ready flag.
		playerGameData<wxInt32>(shReady, numPlayers() - 1) = ready;

		// Each player gets a slot that can hold their network drop state.
		playerGameData<HashString>(shNetworkDropFlag, numPlayers() - 1) = shEmpty;

		const Player &player = object.read<Player>();
		ColorType color = object.read<ColorType>(1);

		// They may have been added with no color and been assigned a color
		// by the game.
		if(CR_SIZE == color)
		{
			DataObject input(player), output;
			RULE.Decide(shNetworkLogicPlayerColor, input, output);
			color = output.read<ColorType>();
		}

		// Update the UI.
		RULE.Execute(shNetworkRuleChangeColorMessage, DataObject(player, color));

		// Reset the ready state of every player in the game, as players may
		// want to decide if they want to play with this new player.
		RULE.Execute(shNetworkRuleResetReady, DataObject());
	}
};

IMPLEMENT_RULE(NetworkRuleAddPlayer, NETWORK)
