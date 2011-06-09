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
#include "NetworkPlayers.h"
#include "INetworkEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkRuleServerDesyncCheck
// 
// Sends desync information to clients for them to validate against their data.
//
// Project:
//     <NetworkRules>
//
// RuleSet:
//     <Network>
//
class NetworkRuleServerDesyncCheck : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// Basically, we send rules directly for the client with stuff packed
		// into the DataObject for them to read and check against their
		// game data.

		typedef std::map<HashString, wxInt32> ResourceMap;
			
		// Player resources.
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			ResourceMap resourceMap;

			// Fill in the resource map with the players resources.
			const Data::IntHash& resources = playerGameData<Data::IntHash>(
				shResources, i);

			Data::IntHash::const_iterator it, itEnd = resources.end();
			for(it = resources.begin(); it != itEnd; ++it)
			{
				const HashString& name = it->first;
				wxInt32 total = it->second;

				resourceMap[name] = total;
			}

			DataObject object(i, resourceMap);

			NET.SendRule(shNetworkRuleClientDesyncCheckPlayerResources, object);
		}

		// Bank resources.
		ResourceMap resourceMap;

		// Fill in the resource map with the bank resources.
		const Data::IntHash& resources = gameData<Data::IntHash>(
			shBankResources);

		Data::IntHash::const_iterator it, itEnd = resources.end();
		for(it = resources.begin(); it != itEnd; ++it)
		{
			const HashString& name = it->first;
			wxInt32 total = it->second;

			resourceMap[name] = total;
		}

		DataObject object(resourceMap);

		NET.SendRule(shNetworkRuleClientDesyncCheckBankResources, object);
	}
};

IMPLEMENT_RULE(NetworkRuleServerDesyncCheck, NETWORK)
