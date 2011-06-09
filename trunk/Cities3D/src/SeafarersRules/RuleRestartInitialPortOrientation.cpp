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
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRestartInitialPortOrientation
// 
// Mixes into <RuleRestartGame>.  Restarts the game from 
// the <Game> State of InitialPortOrientation.
//
// Derived From:
//     <Rule>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <PortPlacement>
//
// Mixin To:
//     <RuleRestartGame>
//
// Rules Called:
//     <RuleRequestPortOrientation>
//
class RuleRestartInitialPortOrientation : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		//just run the request for an initial port again
		RULE.Execute(shRuleRequestPortOrientation, 
			DataObject(gameData<wxInt32>(shInitialPortTile)));
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RuleRestartInitialPortOrientation, 
						   InitialPortOrientation, RuleRestartGame, 
						   PORT_PLACEMENT);
