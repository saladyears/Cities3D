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

//---------------------------------------------------------------------------//
// Class: NetworkRuleGenerateSystemMessage
// 
// Generates a system message in network games.
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
//     <RuleGenerateSystemMessage>
//
class NetworkRuleGenerateSystemMessage : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		NET.SendRule(shNetworkRuleSystemMessage, object);
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(NetworkRuleGenerateSystemMessage, Network, 
						   RuleGenerateSystemMessage, NETWORK)

// This is kind of distressing, but the game mode isn't set to Network until it
// actually gets initialized.  Since this rule isn't loaded in Hotseat games,
// it all works out, it's just grating to have to put this extra key in.
IMPLEMENT_RULE_KEYED_MIXIN(NetworkRuleGenerateSystemMessage, Hotseat, 
						   RuleGenerateSystemMessage, NETWORK)
