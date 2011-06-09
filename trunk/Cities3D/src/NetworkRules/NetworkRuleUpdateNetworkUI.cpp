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
#include "HTML.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkRuleUpdateNetworkUI
// 
// Sends a system message with the given text in a network game.
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
//     <RuleUpdateNetworkUI>
//
// Rules Called:
//     <RuleGenerateSystemMessage>
//
class NetworkRuleUpdateNetworkUI : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxInt32 index = -1;
		if(2 <= object.numItems())
		{
			index = object.read<wxInt32>(1);
		}

		const wxString& message = object.read<wxString>();
		const wxString& name = 
			playerGame(index).player().Name();

		// Use the given string as the format string and add the player's name.
		wxString str = wxString::Format(message.c_str(), name.c_str());

		HTML::Color(str, playerGame(index).color());

		RULE.Execute(shRuleGenerateSystemMessage, DataObject(str));
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(NetworkRuleUpdateNetworkUI, Network, 
						   RuleUpdateNetworkUI, NETWORK)
