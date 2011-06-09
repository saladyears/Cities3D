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
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleSelectCommercialHarborCommodity
// 
// Handles the player selecting the commodity to trade via the Commercial 
// Harbor.
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
class RuleSelectCommercialHarborCommodity : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
        // This player has ended a blocking action.
        wxInt32 victim = gameData<wxInt32>(shCommercialHarborVictim);
        RULE.Execute(shRuleEndBlockingAction, DataObject(victim));

		// Record their selection and see if we can continue on.
		typedef std::map<HashString, wxInt32> ResourceMap;
		const ResourceMap& resources = object.read<ResourceMap>();
		wxASSERT(1 == resources.size());

		const HashString& res = resources.begin()->first;
		gameData<HashString>(shCommercialHarborCommodity) = res;

		RULE.Execute(shRuleRestartTurnCommercialHarborTrade, 
			DataObject(false));
	}
};

IMPLEMENT_RULE(RuleSelectCommercialHarborCommodity, CITIES)
