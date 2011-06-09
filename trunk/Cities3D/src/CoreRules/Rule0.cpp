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
#include "Controller.h"
#include "IPluginEngine.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: Rule0
// 
// The starting point of a game.  Everything that happens prior to
// Rule0 is related to setting up the parameters and players that
// will participate in that game.  Everything that happens after
// Rule0 is related to what actually happens in that game.  Calls
// <RuleGenerateBoard> to create and prepare the <Game> and <Map>
// data.  Transmits the signal to initialize the View via the
// eventInitializeUI <Event>.  Starts off along the rule chain
// by seguing into <RuleMarkerStartInitial>.
//
// Derived From:
//     <Rule>
//
// Project:
//     <CoreRules>
//
// RuleSet:
//     <Core>
//
// Rules Called:
//     <RuleGenerateBoard>,
//     <RuleMarkerStartInitial>
//
// Transmits To:
//     eventInitializeUI
//
class Rule0 : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		//generate the board
		RULE.Execute(shRuleGenerateBoard, DataObject());

		//the board is generated, now get the UI ready
		Controller::get().Transmit(shEventInitializeUI, GetGame());

		RULE.Execute(shRuleMarkerStartInitial, DataObject());

		// Get all of the loaded rulesets for this game.
		IPluginEngine::StringPairArray array;
		PLUGIN.GetLoadedRuleSets(array);
	}
};

IMPLEMENT_RULE(Rule0, CORE);
