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
#include "Map.h"
#include "IRuleEngine.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkRuleLoadMap
// 
// Loads a new map in a network game.
//
// Derived From:
//     <Rule>
//
// Project:
//     <NetworkRule>
//
// RuleSet:
//     <Network>
//
// Rules Called:
//     <RuleLoadMap>
//     <RuleGenerateSystemMessage>
//
class NetworkRuleLoadMap : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		RULE.Execute(shRuleLoadMap, object);

		// Fire off the system message.
		wxString str = wxString::Format(stTheMapIsNow.c_str(),
			MAP.name().c_str());
		RULE.Execute(shRuleGenerateSystemMessage, DataObject(str));
	}
};

IMPLEMENT_RULE(NetworkRuleLoadMap, NETWORK)
