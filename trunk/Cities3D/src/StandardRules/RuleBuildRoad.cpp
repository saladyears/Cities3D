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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleBuildRoad
// 
// Mixin container for handling a user click on the road <BuildUI>.  Looks up 
// the current <Game> State in its <IRule> hash and executes that <IRule> if 
// one exists.  If no such <IRule> exists for the current <Game> State, an 
// error is logged to the screen.
//
// Derived From:
//     <Rule>,
//     <RuleHashMixin>
//     <IUndoable>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
class RuleBuildRoad 
	: public Rule
	, public RuleHashMixin
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
		const HashString& state = gameData<HashString>(shState);
		
		if(false == ExecuteHash(state, object))
		{
			wxLogError(
				wxT("Programmer Error: No State %s in RuleBuildRoad"), 
				state.cwx_str());
		}
	}
	
	virtual void Unexecute(const DataObject&)
	{
		// Does nothing.
	}

	virtual bool CanUndo() const
	{
		// This needs to be marked as undoable so that auto-trades can be
		// undone.
		return true;
	}
};

IMPLEMENT_RULE(RuleBuildRoad, STANDARD)
