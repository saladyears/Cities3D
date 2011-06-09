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
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkRuleServerStart
// 
// Sends out the appropriate notifications when the server starts going.
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
// Transmits To:
//     eventGameConnect
//     eventSpectator
//
class NetworkRuleServerStart : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// Now that we're successfully listening, tell the UI we can get the 
		// game going.
		Controller::get().Transmit(shEventGameConnect, DataObject(true));

		// We have to send the spectator event again after the game connect 
		// event because the spectator UI isn't hooked up until that event 
		// fires.
		Controller::get().Transmit(shEventSpectator, 0);
	}
};

IMPLEMENT_RULE(NetworkRuleServerStart, NETWORK)
