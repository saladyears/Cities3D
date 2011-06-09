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
// Class: RuleExecuteMerchantFleetCard
// 
// Makes the given resource or commodity tradable at 2-1 for this turn.
//
// Derived From:
//     <Rule>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
class RuleExecuteMerchantFleetCard : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
        // The current player has ended a blocking action.
        RULE.Execute(shRuleEndBlockingAction, DataObject(current()));

		wxASSERT(1 <= object.numItems());

		typedef std::map<HashString, wxInt32> ResourceMap;
		const ResourceMap& resource = object.read<ResourceMap>();
		wxASSERT(1 == resource.size());

		const HashString& res = resource.begin()->first;

		DataObject input(res), output;
		RULE.Decide(shLogicResourceName, input, output);
		const wxString& resName = output.read<wxString>();

		wxString str = wxString::Format(
			stUsesTheMerchantFleet.c_str(), 
            swStringFormat.c_str(), resName.c_str());
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(str));

		// Mark this resource as tradeable 2-1.
		playerGameData<HashStringArray>(shMerchantFleetCards).push_back(res);

		RULE.Execute(shRulePlayProgressCardEnd, DataObject());
	}
};

IMPLEMENT_RULE(RuleExecuteMerchantFleetCard, CITIES)
