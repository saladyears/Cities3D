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
#include "PlayerData.h"
#include "RuleSetDatabase.h"
#include "LogicBranchDefault.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------------------------------------------------------//
// Class: LogicGold
// 
// Runs the rule that looks for gold that needs to be spent.
//
// Derived From:
//     <Logic>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Gold>
//
// Game State:
//     Gold
//
class LogicGold : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		gameData<HashString>(shState) = shGold;

		output = DataObject(shRuleGold);
	}
};

//---------------------------- BRANCH LOGIC     -----------------------------//
// Gold is handled after resources are gathered, which is at priority 100.
IMPLEMENT_PRIORITY_BRANCH(GOLD, RuleMarkerPostDiceRoll, LogicGold, 50);
