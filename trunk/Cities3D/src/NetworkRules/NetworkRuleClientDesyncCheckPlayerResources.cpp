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
// Class: NetworkRuleClientDesyncCheckPlayerResources
// 
// Checks desync information related to player resources sent from the server.
//
// Project:
//     <NetworkRules>
//
// RuleSet:
//     <Network>
//
class NetworkRuleClientDesyncCheckPlayerResources : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		typedef std::map<HashString, wxInt32> ResourceMap;
			
		wxInt32 playerIndex = object.read<wxInt32>();
		const ResourceMap& resources = object.read<ResourceMap>(1);
		
		wxInt32 desyncs = 0;
		wxString output;

		// Check against this player's data.
		ResourceMap::const_iterator it, itEnd = resources.end();
		for(it = resources.begin(); it != itEnd; ++it)
		{
			const HashString& name = it->first;
			wxInt32 total = it->second;

			wxInt32 playerTotal = playerGameData<Data::IntHash>(
				shResources, playerIndex)[name];

			// Desync detected.
			if(total != playerTotal)
			{
				// Don't bother translating these.
				if(0 < desyncs)
				{
					output += swSpace;
				}

				output += wxString::Format(swIntOnServerIntOnClient.c_str(),
					name.cwx_str(), total, playerTotal);

				++desyncs;
			}
		}

		if(0 < desyncs)
		{
			output = wxString::Format(swDesyncDetectedString.c_str(), 
				playerGame(playerIndex).player().Name().c_str()) + output;
			wxLogError(output);
		}
	}
};

IMPLEMENT_RULE(NetworkRuleClientDesyncCheckPlayerResources, NETWORK)
