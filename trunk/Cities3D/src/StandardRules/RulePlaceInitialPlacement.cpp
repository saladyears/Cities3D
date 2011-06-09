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
#include "RuleHashMixin.h"
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
// Class: RulePlaceInitialPlacement
// 
// Mixin container for placing an initial placement on 
// the board. Looks up the current <Game> State in its 
// <IRule> hash and executes that <IRule> if one exists.  
// If no such <IRule> exists for the current <Game> State, 
// an error is logged to the screen. *Undoable*.
//
// Derived From:
//     <Rule>
//     <RuleHashMixin>
//     <IUndoable>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// Rules Called:
//     <RulePlaceInitialExtensions>
//
// Transmits To:
//     eventPlayerUI
//
class RulePlaceInitialPlacement 
: public Rule
, public RuleHashMixin
, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
        // The current player has ended a blocking action.
        RULE.Execute(shRuleEndBlockingAction, DataObject(current()));

		const HashString& state = gameData<HashString>(shState);
		if(false == ExecuteHash(state, object))
		{
			wxLogError(
				wxT("Programmer Error: No State %s in RulePlaceInitialPlacement"), 
				state.cwx_str());

			return;
		}

		//call any extensions needed
		RULE.Execute(shRulePlaceInitialExtensions, object);

		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), -1));
		
		//trust the rule chain to get us to the next rule
	}

	virtual void Unexecute(const DataObject &)
	{
		//undo is done in reverse order automatically

		//fire the update
		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), -1));
	}

	virtual bool CanUndo() const
	{
		//initial placements are *always* undoable
		return true;
	}
};

IMPLEMENT_RULE(RulePlaceInitialPlacement, STANDARD);
