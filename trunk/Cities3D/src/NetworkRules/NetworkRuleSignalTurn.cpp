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
// Class: NetworkRuleSignalTurn
// 
// Handles network specific things at the start of a turn.
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
//     <RuleSignalTurn>
//
class NetworkRuleSignalTurn : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// Play the "your turn" sound if this is the right player.
		DataObject input(-1), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);
		if(true == output.read<bool>())
		{
			RULE.Execute(shRulePlaySound, DataObject(SOUND_YOUR_TURN));

			Controller::get().Transmit(shEventRequestUserAttention, 0);
		}
	}
};

IMPLEMENT_RULE_MIXIN(NetworkRuleSignalTurn, RuleSignalTurn, NETWORK)
