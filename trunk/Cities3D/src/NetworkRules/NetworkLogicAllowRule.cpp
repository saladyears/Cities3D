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
#include "Logic.h"
#include "RuleSetDatabase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	WX_DECLARE_HASH_STRING_HASH_SET(StringSet);

	StringSet sAllowedRules;
}

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkLogicAllowRule
// 
// Determines whether the given rule is allowed to be executed.
//
// Derived From:
//     <Logic>
//
// Project:
//     <NetworkRules>
//
// RuleSet:
//     <Network>
//
class NetworkLogicAllowRule : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		// If this is the first time through, fill in the allowed rules.
		if(true == sAllowedRules.empty())
		{
			sAllowedRules.insert(shNetworkRuleChatMessage); 
			sAllowedRules.insert(shNetworkRuleTakeSeat);
			sAllowedRules.insert(shNetworkRuleQuit);
			sAllowedRules.insert(shNetworkRuleAllSeatsFull);
			sAllowedRules.insert(shNetworkRuleConnectionLost);
			sAllowedRules.insert(shNetworkRuleAddSpectator);
			sAllowedRules.insert(shNetworkRuleSystemMessage);
			sAllowedRules.insert(shNetworkRuleSpectatorState);
			sAllowedRules.insert(shNetworkRulePlayerReplace);
			sAllowedRules.insert(shRuleRestartGame);
			sAllowedRules.insert(shRulePlaySound);
		}

		const HashString &rule = input.read<HashString>();
		const HashString &state = gameData<HashString>(shState);

		wxLogDebug(wxT("NetworkLogicAllowRule evaluating rule %s in state %s."), 
			rule.cwx_str(), state.cwx_str());

		bool allow = true;

		// If we are in NetworkRestart mode, we have to reject every rule 
		// except chat messages, quit messages, and rejoin messages.  This 
		// prevents synch issues where someone drops but someone else sends a 
		// rule at the same time.
		if( (shNetworkRestart == state) &&
			(sAllowedRules.end() == sAllowedRules.find(rule)))
		{
			allow = false;
		}

		output = DataObject(allow);
	}
};

IMPLEMENT_LOGIC(NetworkLogicAllowRule, NETWORK)
