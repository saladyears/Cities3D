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
#include "CornerObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicIsValidDiceRollNo7s
// 
// Disallows 7s during a certain number of opening rounds.
//
// Derived From:
//     <Logic>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <No7s>
//
// Mixin To:
//     <LogicIsValidDiceRoll>
//
class LogicIsValidDiceRollNo7s : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		wxASSERT(2 <= input.numItems());

		bool valid = true;

		size_t die1 = input.read<size_t>();
		size_t die2 = input.read<size_t>(1);

		if(7 == (die1 + die2))
		{
			wxInt32 turn = gameData<wxInt32>(shTurn);
			wxInt32 players = numPlayers();

			wxInt32 round = ((turn - 1) / players) + 1;
			wxInt32 rounds = gameData<wxInt32>(shNo7Rounds);

			if(rounds >= round)
			{
				valid = false;
			}
		}

		output = DataObject(valid);
	}
};

IMPLEMENT_LOGIC_MIXIN(LogicIsValidDiceRollNo7s, LogicIsValidDiceRoll, NO7S);
