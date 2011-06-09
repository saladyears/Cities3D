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
#include "RuleSetDatabase.h"
#include "GameEngine.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRequestInitialRoadSeafarers
// 
// Prepares locations for an initial road or ship.  Replaces 
// <RuleRequestInitialRoad>.  Acts in all ways like <RuleRequestInitialRoad>,
// but replaces "road" with "ship" if required in all <IRules>
// and <ILogic> called.
//
// Derived From:
//     <Rule>,
//     <LogicHashMixin>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <Seafarers>
//
// Replaces:
//     <RuleRequestInitialRoad>
//
// Rules Called:
//     <RuleUpdateMessageUI>
//     <RuleLocateInitialRoad>
//
class RuleRequestInitialRoadSeafarers 
: public Rule
, public LogicHashMixin
{
public:
	virtual void Execute(const DataObject &object)
	{
		// The current player is now in a blocking action.
        RULE.Execute(shRuleBeginBlockingAction, DataObject(current()));

		wxASSERT(1 <= object.numItems());

		//retrieve the text to put in the MessageUI
		DataObject input(0), output;
		
		const HashString& state = gameData<HashString>(shState);
		if(false == DecideHash(state, input, output))
		{
			wxLogError(
				wxString::Format(wxT("Programmer Error: No State %s in ")
				wxT("RuleRequestInitialRoadSeafarers"), state.cwx_str()));

			return;
		}

		wxASSERT(3 == output.numItems());

		wxString text1 = output.read<wxString>();
		wxString text2 = output.read<wxString>(1);
		HashString logic = HashString::Format(shLogicStringFormat.c_str(),
			output.read<HashString>(2).c_str());
		HashString rule = HashString::Format(shRuleStringFormat.c_str(),
			output.read<HashString>(2).c_str());

		//if this involves ships, do some crazy mojo
		if(TRUE == gameData<wxInt32>(shInitialShip))
		{
			text1 = stPlaceAShip;
			text2 = stWaitingPlaceAShip;

			wxInt32 index = logic.find(shRoad);
			wxASSERT(-1 != index);

			logic.replace(index, 4, shShip);
            logic = HashString(logic.c_str());
		}

		RULE.Execute(shRuleUpdateMessageUI, 
			DataObject(text1, text2));		
		
		RULE.Execute(shRuleLocateInitialRoad, DataObject(
			object.read<wxInt32>(), logic, rule));
	}
};

IMPLEMENT_RULE_REPLACE(RuleRequestInitialRoadSeafarers, RuleRequestInitialRoad,
					   SEAFARERS);
