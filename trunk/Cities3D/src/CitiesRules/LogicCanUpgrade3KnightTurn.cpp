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
// Class: LogicCanUpgrade3KnightTurn
// 
// Determines whether a knight can be built during the player's turn.
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
//     <LogicCanUpgrade3Knight>
//
// Logic Called:
//     <LogicCanPurchaseKnight>
//
class LogicCanUpgrade3KnightTurn : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		// There are four conditions that must be met in order for a player to
		// be allowed to upgrade to a mighty knight:
		// 
		// 1) They must have politics upgraded to level 3.
		// 2) They must have at least one mighty knight in stock.
		// 3) They must have the resources to purchase a knight.
		// 4) They must have a strong knight on the board to replace.
		//
		// None of these conditions can be affected by other rulesets.

		bool canBuild = false;
		wxInt32 trades = 0;

		// Conditions 1 and 2.
		if( (3 <= playerGameData<wxInt32>(shPoliticsLevel)) &&
			(0 < playerGameData<wxInt32>(shStockKnights3)))
		{
			// Condition 3.
			RULE.Decide(shLogicCanPurchaseKnightUpgrade, input, output);
			wxASSERT(2 <= output.numItems());

			if(true == output.read<bool>())
			{
				trades = output.read<wxInt32>(1);

				// Condition 4.
				output.reset();
				RULE.Decide(shLogicCanPlace3Knight, input, output);

				canBuild = (false == 
					output.read<PlayerGame::CornerObjectArray>().empty());
			}
		}

		output = DataObject(canBuild, trades);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCanUpgrade3KnightTurn, Turn,
							LogicCanUpgrade3Knight, CITIES)
