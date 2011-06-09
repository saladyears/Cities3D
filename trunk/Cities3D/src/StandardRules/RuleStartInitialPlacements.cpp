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
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleStartInitialPlacements
// 
// Default path in the rule chain after <RuleMarkerStartInitial>.  Sets
// the <Game> State to FirstSettlement and lets the rule chain continue
// on.
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
// Game State:
//     FirstSettlement
//
// Transmits To:
//     eventControlsUI
//
class RuleStartInitialPlacements : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		//we're in a new state
		gameData<HashString>(shState) = shFirstSettlement;

		//show the ControlsUI now that we're starting
		Controller::get().Transmit(shEventControlsUI, DataObject(true));

		// Let the rule chain send us on.
	}
};

IMPLEMENT_RULE(RuleStartInitialPlacements, STANDARD);
