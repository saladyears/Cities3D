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
#include "LogicHashMixin.h"
#include "IUndoable.h"
#include "RuleSetDatabase.h"
#include "GameEngine.h"
#include "Controller.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePlaceInitialRoadSeafarers
// 
// Places an initial road or ship on the board.  Replaces
// <RulePlaceInitialRoad>.  Acts in all ways like <RulePlaceInitialRoad>,
// except the placement <IRule> called has its text changed
// from "Road" to "Ship" if required.
//
// Derived From:
//     <Rule>,
//     <LogicHashMixin>
//     <IUndoable>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <Seafarers>
//
// Replaces:
//     <RulePlaceInitialRoad>
//
// Transmits To:
//     eventPlayerUI
//
class RulePlaceInitialRoadSeafarers 
	: public Rule
	, public LogicHashMixin
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
		const HashString& state = gameData<HashString>(shState);
		DataObject output;

		if(false == DecideHash(state, object, output))
		{
			wxLogError(
				wxString::Format(wxT("Programmer Error: No State %s in ")
				wxT("RulePlaceInitialRoad"), state.cwx_str()));

			return;
		}

		wxASSERT(3 <= output.numItems());

		HashString rule = output.read<HashString>();
		
		//check for ship placements
		if(TRUE == gameData<wxInt32>(shInitialShip))
		{
			wxInt32 index = rule.find(shRoad);
			wxASSERT(-1 != index);

			rule.replace(index, 4, shShip);
            rule = HashString(rule.c_str());
		}

		RULE.Execute(rule, object);

		RULE.Execute(output.read<HashString>(1), 
			DataObject(output.read<HashString>(2)));

		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), -1));
		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), -2));
		
		// Every road placement counts as a turn.
		RULE.Execute(shRuleIncrementTurnsPlayed, DataObject());

		//trust the rule chain to get us to the next rule
	}

	virtual void Unexecute(const DataObject &)
	{
		wxASSERT(false);
	}

	virtual bool CanUndo() const
	{
		// Initial road placements cannot be undone.
		return false;
	}
};

IMPLEMENT_RULE_REPLACE(RulePlaceInitialRoadSeafarers, 
					   RulePlaceInitialRoad, SEAFARERS);
