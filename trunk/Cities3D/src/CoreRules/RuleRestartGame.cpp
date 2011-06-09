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
#include "RuleHashMixin.h"
#include "GameEngine.h"
#include "Controller.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRestartGame
// 
// Restarts the game from a saved point on disk.  Calls
// <RulePrepareUI> to start the UI up again.  Executes the <IRule> in 
// its hash that corresponds to the <Game>'s State field.  If no such
// <IRule> exists in the hash, an error is thrown to the screen and
// the game cannot be played from the saved point.
//
// Derived From:
//     <Rule>,
//     <RuleHashMixin>
//
// Project:
//     <CoreRules>
//
// RuleSet:
//     <Core>
//
// Rules Called:
//      <RulePrepareUI>
//
class RuleRestartGame 
	: public Rule
	, public RuleHashMixin
{
public:
	virtual void Execute(const DataObject &object)
	{
		RULE.Execute(shRuleResetTileFlags, DataObject());
		RULE.Execute(shRuleResetObjectFlags, DataObject());

		//restart the game with the current state
		const HashString& state = gameData<HashString>(shState);
		if(false == ExecuteHash(state, object))
		{
			wxLogError(wxT("Programmer Error: No State %s in RuleRestartGame"), 
				state.cwx_str());

			//update the text so the user can see what's going on if they 
			//somehow miss the above message
			Controller::get().Transmit(shEventMessageUI,
				DataObject(stUnableToRestartTheGame, GetGame()));
		}
	}
};

IMPLEMENT_RULE(RuleRestartGame, CORE);
