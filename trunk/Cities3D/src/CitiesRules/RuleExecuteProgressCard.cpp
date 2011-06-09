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
#include "RNG.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleExecuteProgressCard
// 
// Doles out progress cards to people.
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
class RuleExecuteProgressCard : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(2 <= object.numItems());

		typedef std::map<HashString, wxInt32> ResourceMap;
		const ResourceMap& map = object.read<ResourceMap>();
		wxASSERT(1 == map.size());

		HashString cardType = map.begin()->first;

		wxInt32 index = cardType.find(shCard);
		wxASSERT(-1 != index);
		cardType.replace(index, 4, shEmpty);

		index = object.read<wxInt32>(1);

        // This player has ended a blocking action.
        RULE.Execute(shRuleEndBlockingAction, DataObject(index));

		// Mark this player as having received their card.
		playerGameData<wxInt32>(shGainProgressCard, index) = FALSE;

		// Give them their card.
		RULE.Execute(shRuleProgressCardCommon, DataObject(index, cardType));

		// See if there are any more to be selected
		RULE.Execute(shRuleSelectProgressCards, DataObject());
	}
};

IMPLEMENT_RULE(RuleExecuteProgressCard, CITIES)
