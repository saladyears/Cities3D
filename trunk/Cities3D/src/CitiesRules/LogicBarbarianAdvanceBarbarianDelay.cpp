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
#include "Logic.h"
#include "RuleSetDatabase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicBarbarianAdvanceBarbarianDelay
// 
// Prevents the barbarians from advancing during the first X turns.
//
// Derived From:
//     <Logic>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <BarbarianDelay>
//
// Mixin To:
//     <LogicBarbarianAdvance>
//
class LogicBarbarianAdvanceBarbarianDelay : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		bool valid = true;

		wxInt32 turn = gameData<wxInt32>(shTurn);
		wxInt32 players = numPlayers();

		wxInt32 round = ((turn - 1) / players) + 1;
		wxInt32 rounds = gameData<wxInt32>(shBarbarianDelayRounds);

		if(rounds >= round)
		{
			valid = false;
		}

		output = DataObject(valid);
	}
};

IMPLEMENT_LOGIC_MIXIN(LogicBarbarianAdvanceBarbarianDelay, 
					  LogicBarbarianAdvance, CITIES)
