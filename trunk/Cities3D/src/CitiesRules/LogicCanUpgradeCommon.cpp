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
// Class: LogicCanUpgradeCommon
// 
// Common code for handling city upgrades.
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
class LogicCanUpgradeCommon : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		const HashString& type = input.read<HashString>();
		HashString levelType = HashString::Format(shStringLevel.c_str(), 
            type.c_str());
		HashString playerType = HashString::Format(shStringPlayer.c_str(), 
            type.c_str());

		// There are three conditions that must be met in order for a player to
		// be allowed to upgrade:
		// 
		// 1) They must have at least one city on the board.
		// 2) They must be at level 5 or lower in the upgrade type.
		// 3) They must have a city on which to place a metropolis if they
		//    are going for level 4 or 5, unless they already have that
		//    particular metropolis.
		// 4) They must have the resources available to upgrade.
		// 
		// None of these conditions can be affected by other rulesets.

		bool canUpgrade = false;
		wxInt32 trades = 0;

		// Condition 1.
		const PlayerGame::CornerObjectArray& cities = 
			playerGame().getCornerObjects(shCities);

		if(false == cities.empty())
		{
			// Condition 2.
			wxInt32 level = playerGameData<wxInt32>(levelType);
			if(5 > level)
			{
				// Condition 3.
				const PlayerGame::CornerObjectArray& metros =
					playerGame().getCornerObjects(shMetropolis);
				size_t metroSize = metros.size();
				size_t citiesSize = cities.size();

				wxInt32 metroOwner = gameData<wxInt32>(playerType);

				if( (3 > level) ||
					(metroOwner == current()) ||
					(citiesSize > metroSize))
				{
					// Condition 4.
					RULE.Decide(shLogicCanPurchaseUpgrade, DataObject(type), 
						output);
					wxASSERT(2 <= output.numItems());

					canUpgrade = output.read<bool>();

					if(true == canUpgrade)
					{
						trades = output.read<wxInt32>(1);
					}
				}
			}
		}

		output = DataObject(canUpgrade, trades);
	}
};

IMPLEMENT_LOGIC(LogicCanUpgradeCommon, CITIES)
