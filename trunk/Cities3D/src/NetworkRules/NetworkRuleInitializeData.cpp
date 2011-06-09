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
#include "GameEngine.h"
#include "NetworkPlayers.h"
#include "INetworkEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------------------------------------------------------//
// Class: NetworkRuleInitializeData
// 
// Initialize starting data for a network game.
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
// Mixin To:
//     <RuleInitializeData>
//
class NetworkRuleInitializeData : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// Save off the host player's ID.
		gameData<wxInt32>(shHostID) =
			NET.Players().GetHostPlayer().Id();

		// Set the mode to network.
		gameData<HashString>(shMode) = shNetwork;

		// Set the countdown timer for each player to the off position.
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			playerGameData<wxTimeSpan>(shRestartTime, i) = 
				wxTimeSpan(0, 0, 0, -1);
		}
	}
};

IMPLEMENT_RULE_MIXIN(NetworkRuleInitializeData, RuleInitializeData, NETWORK)
