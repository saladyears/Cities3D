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
// Class: LogicCanBuildRoadTurn
// 
// Determines whether a road can be built during the player's turn.
//
// Derived From:
//     <Logic>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// Mixin To:
//     <LogicCanBuildRoad>
//
// Logic Called:
//     <LogicCanPurchaseRoad>
//
class LogicCanBuildRoadTurn : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		// There are three conditions that must be met in order for a player to
		// be allowed to build a road:
		// 
		// 1) They must have at least one road in stock.
		// 2) They must have the resources to purchase a road.
		// 3) They must have a location on the board to place the road.
		//
		// Condition 1 is static and cannot be affected by any other rulesets.
		// Condition 2 may be affected by other rulesets (i.e. Road Building 
		// card).
		// Condition 3 may be affected by other rulesets (i.e. Seafarers).

		bool canBuild = false;
		wxInt32 trades = 0;

		// Condition 1.
		if(0 < playerGameData<wxInt32>(shStockRoads))
		{
			// Condition 2.
			RULE.Decide(shLogicCanPurchaseRoad, input, output);
			wxASSERT(2 <= output.numItems());

			if(true == output.read<bool>())
			{
				trades = output.read<wxInt32>(1);

				// Condition 3.
				output.reset();
				RULE.Decide(shLogicCanPlaceRoad, input, output);

				typedef std::pair<wxInt32, wxInt32> TileSide;
				typedef std::set<TileSide> TileSideSet;

				canBuild = (false == output.read<TileSideSet>().empty());
			}
		}

		output = DataObject(canBuild, trades);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCanBuildRoadTurn, Turn, LogicCanBuildRoad, 
							STANDARD)
