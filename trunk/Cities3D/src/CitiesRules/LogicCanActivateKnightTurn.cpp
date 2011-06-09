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
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicCanActivateKnightTurn
// 
// Determines whether a knight can be activated during the player's turn.
//
// Derived From:
//     <Logic>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
// Mixin To:
//     <LogicCanActivateKnight>
//
// Logic Called:
//     <LogicCanPurchaseActivateKnight>
//
class LogicCanActivateKnightTurn : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		// There are two conditions that must be met in order for a player to
		// be allowed to build a knight:
		// 
		// 1) They must have the resources to activate a knight.
		// 2) They must have at least one inactive knight on the board
		//
		// None of these conditions can be affected by other rulesets.

		bool can = false;
		wxInt32 trades = 0;

		// Condition 1.
		RULE.Decide(shLogicCanPurchaseActivateKnight, input, output);
		wxASSERT(2 <= output.numItems());

		if(true == output.read<bool>())
		{
			trades = output.read<wxInt32>(1);

			// Condition 2.
			output.reset();
			RULE.Decide(shLogicCanPlaceActivateKnight, input, output);

			can = (false == 
				output.read<PlayerGame::CornerObjectArray>().empty());
		}

		output = DataObject(can, trades);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCanActivateKnightTurn, Turn,
							LogicCanActivateKnight, CITIES)
