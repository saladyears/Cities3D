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
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkRuleConnectionLostDrop
// 
// Handles a player dropping from a network game.
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
//     <NetworkRuleConnectionLost>
//
// Rules Called:
//     <NetworkRuleConnectionLostPreGame>
//     <RuleGenerateSystemMessage>
//
class NetworkRuleConnectionLostDrop : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		// Inform the UI.
		const Player &player = object.read<Player>();
		wxString str = wxString::Format(stDropsFromTheGame.c_str(),
			player.Name().c_str());
		RULE.Execute(shRuleGenerateSystemMessage, DataObject(str));

		const HashString &state = gameData<HashString>(shState);

		// Handle pre game vs. in game.
		if(shNew == state)
		{
			RULE.Execute(shNetworkRuleConnectionLostPreGame, object);
		}
		else if(shComplete != state)
		{
			RULE.Execute(shNetworkRuleConnectionLostInGame, object);
		}
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(NetworkRuleConnectionLostDrop, Drop,
						   NetworkRuleConnectionLost, NETWORK)
