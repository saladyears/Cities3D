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
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePlayCardCollection
// 
// Plays a collection card.
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
// Mixin To:
//     <RulePlayCard>
//
class RulePlayCardCollection : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(2 <= object.numItems());

		const HashString& type = object.read<HashString>();
		const wxString& text = object.read<wxString>(1);
		
		// The player gets 2 resources for every tile they have a city or
		// settlement on.
		DataObject input(type), output;
		RULE.Decide(shLogicIsCardPlayableCollection, input, output);

		typedef std::set<wxInt32> IntSet;
		const IntSet& tiles = output.read<IntSet>();
		
		// The player gets up to two times the amount of tiles they are on.
		wxInt32 amount = tiles.size() * 2;
		
		RULE.Execute(shRuleBankTransact, DataObject(current(), type, amount));

		Controller::get().Transmit(shEventPlayerUI, DataObject(GetGame(), 
			current()));

		output.reset();
		RULE.Decide(shLogicResourceName, input, output);
		const wxString& name = output.read<wxString>();

		wxString str = wxString::Format(text, swStringFormat.c_str(), amount, 
            name.c_str());
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(str));

		RULE.Execute(shRulePlayProgressCardEnd, DataObject());
	}
};

IMPLEMENT_RULE(RulePlayCardCollection, CITIES)
