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
// Class: RuleRequestInitialRoad
//
// Mixin container for finding initial road positions from
// the given initial placement location.  Looks up the current 
// <Game> State in its  <ILogic> hash and calls that <ILogic> if 
// one exists.  The hashed <ILogic> must provide the test to pass to
// the <MessageUI> in the first output <DataObject> position, the 
// name of the <ILogic> that will evaluate the possible road positions
// in the second output <DataObject> position, and the name of the
// <IRule> that will handle the user's road selection in the third output
// <DataObject> position.
//
// Derived From:
//     <Rule>,
//     <LogicHashMixin>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// Rules Called:
//     <RuleUpdateMessageUI>,
//     <RuleLocateInitialRoad>
//
class RuleRequestInitialRoad 
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
				wxT("Programmer Error: No State %s in RuleRequestInitialRoad"), 
				state.cwx_str());

			return;
		}

		wxASSERT(3 == output.numItems());

		RULE.Execute(shRuleUpdateMessageUI, 
			DataObject(output.read<wxString>(), output.read<wxString>(1)));

		HashString logic = HashString::Format(shLogicString.c_str(),
			output.read<HashString>(2).c_str());
		HashString rule = HashString::Format(shRuleString.c_str(),
			output.read<HashString>(2).c_str());

		RULE.Execute(shRuleLocateInitialRoad, DataObject(
			object.read<wxInt32>(), logic, rule));
	}
};

IMPLEMENT_RULE(RuleRequestInitialRoad, STANDARD);
