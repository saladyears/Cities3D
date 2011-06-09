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
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkRuleAutoSave
// 
// Saves the game, if the player is the host.
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
//     <RuleAutoSave>
//
class NetworkRuleAutoSave : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		const HashString& state = gameData<HashString>(shState);

		DataObject input(0), output;
		RULE.Decide(shNetworkLogicAllowSaves, input, output);

		if( (true == output.read<bool>()) &&
			(true == NET.Players().IsHost()) &&
			(shComplete != state))
		{
			GameEngine::get().SaveGame();

			RULE.AsynchExecute(shNetworkRuleSystemMessage, DataObject(stGameSaved, 
				wxDateTime::Now()));

			wxLogDebug(wxT("Game saved: State %s"), state.cwx_str());
		}
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(NetworkRuleAutoSave, Network, 
						   RuleAutoSave, NETWORK)
