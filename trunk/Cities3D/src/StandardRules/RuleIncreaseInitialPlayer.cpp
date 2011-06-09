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
#include "GameEngine.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleIncreaseInitialPlayer
// 
// Increases the current player to the next player during
// the first phase of initial placements.  If the end of 
// the phase has been reached, sets the current player to
// the last and signals <RuleMarkerMiddleInitial> to begin
// the second phase of initial placements.  Otherwise, calls 
// <RuleRequestInitialPlacement> to continue initial placements.  
// The <DataObject> it is given must contain the new <Game> State 
// as its first parameter.
//
// Derived From:
//     <Rule>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// Rules Called:
//     <RuleIncrementPlayer>,
//     <RuleMarkerMiddleInitial>,
//     <RuleRequestInitialPlacement>
//
class RuleIncreaseInitialPlayer : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		//see if we're done with first round selections
		RULE.Execute(shRuleIncrementPlayer, DataObject());

		HashString str;

		//if we're back to the first, it's time to move on to
		//placing second objects
		if(0 == gameData<wxInt32>(shCurrentPlayer))
		{
			gameData<wxInt32>(shCurrentPlayer) = (numPlayers() - 1);

			str = shRuleMarkerMiddleInitial;
		}
		else
		{
			str = shRuleRequestInitialPlacement;
			gameData<HashString>(shState) = object.read<HashString>();
		}

		//keep going
		RULE.Execute(str, DataObject());
	}
};

IMPLEMENT_RULE(RuleIncreaseInitialPlayer, STANDARD);
