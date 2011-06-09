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
// Class: RulePlaceInitialRoad
// 
// Mixin container for placing an initial road on the
// board.  Looks up the current <Game> State in its 
// <ILogic> hash and calls that <ILogic> if one exists.  The
// hashed <ILogic> must provide the name of an <IRule> that
// will handle the object placement in the first output <DataObject> 
// position, the name of an <IRule> that will handle advancing
// to the next player in the second output <DataObject> position,
// and the name of the next <Game> State in the third output
// <DataObject> position.
//
// Derived From:
//     <Rule>,
//     <LogicHashMixin>
//     <IUndoable>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// Transmits To:
//     eventPlayerUI
//
class RulePlaceInitialRoad 
	: public Rule
	, public LogicHashMixin
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
        // The current player has ended a blocking action.
        RULE.Execute(shRuleEndBlockingAction, DataObject(current()));

		const HashString& state = gameData<HashString>(shState);
		DataObject output;

		if(false == DecideHash(state, object, output))
		{
			wxLogError(
				wxT("Programmer Error: No State %s in RulePlaceInitialRoad"), 
				state.cwx_str());

			return;
		}

		wxASSERT(3 <= output.numItems());

		//execute the placement rule
		RULE.Execute(output.read<HashString>(), object);

		//execute the advancement rule with the next game state
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

IMPLEMENT_RULE(RulePlaceInitialRoad, STANDARD);
