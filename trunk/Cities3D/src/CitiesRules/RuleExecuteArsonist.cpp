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
// Class: RuleExecuteArsonist
// 
// Removes the given cards from the given player.
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
class RuleExecuteArsonist : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(2 <= object.numItems());

		typedef std::map<HashString, wxInt32> ResourceMap;
		const ResourceMap& resources = object.read<ResourceMap>();

		wxString str;
		bool first = true;

		wxInt32 curPlayer = object.read<wxInt32>(1);

        // This player has ended a blocking action.
        RULE.Execute(shRuleEndBlockingAction, DataObject(curPlayer));
		
		// Execute the transactions.
		ResourceMap::const_iterator it, itEnd = resources.end();
		for(it = resources.begin(); it != itEnd; ++it)
		{
			const HashString& res = it->first;
			wxInt32 amount = it->second;

			// Adjust counts.
			RULE.Execute(shRuleBankTransact, DataObject(curPlayer, res, 
				-amount));

			DataObject input(res), output;
			RULE.Decide(shLogicResourceName, input, output);
			const wxString& name = output.read<wxString>();

			if(true == first)
			{
				first = false;
			}
			else
			{
				str += swCommaSpace;
			}

			str += wxString::Format(swIntStringFormat.c_str(), amount, name.c_str());
		}

		str = wxString::Format(stLoses.c_str(), swStringFormat.c_str(), 
            str.c_str());
		
		// This player no longer needs to lose cards to the Arsonist.
		playerGameData<wxInt32>(shArsonist, curPlayer) = FALSE;

		RULE.Execute(shRuleUpdateNetworkUI, DataObject(str, curPlayer));

		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), curPlayer));

		// If a player goes to zero cards, the screen needs to be redrawn.
		Controller::get().Transmit(shEventDirty, 0);
		
		// See if more cards need to be lost.
		RULE.Execute(shRuleRestartTurnArsonist, DataObject(false));
	}
};

IMPLEMENT_RULE(RuleExecuteArsonist, CITIES)
