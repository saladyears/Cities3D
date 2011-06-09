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
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRobberSteal
// 
// Looks for settlements or cities on the given tile to steal a resource from.
//
// Derived From:
//     <Rule>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
class RuleRobberSteal : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		DataObject output;
		RULE.Decide(shLogicRobberStealPlayers, object, output);
		typedef std::set<wxInt32> IntSet;
		const IntSet& players = output.read<IntSet>();		

		// If there are any players, the player has a choice to steal or not
		// steal.
		if(1 <= players.size())
		{
			gameData<HashString>(shState) = shRobberSteal;
			RULE.Execute(shRuleRestartRobberSteal, object);
		}
		// Otherwise, mark that they stole nothing.
		else
		{
			RULE.Execute(shRuleUpdateNetworkUI, DataObject(stStealsNothing));

			// We're done, move on.
			RULE.Execute(shRuleRestartOldState, DataObject());
		}
	}
};

IMPLEMENT_RULE(RuleRobberSteal, STANDARD)
