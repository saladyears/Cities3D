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
// Class: LogicCanRepairCity
// 
// Determines whether the player is able to repair a city.
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
class LogicCanRepairCity : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		// There are two conditions that must be met in order for a player to
		// be allowed to repair a city:
		// 
		// 1) They must have at least one sabotaged city.
		// 2) They must have the resources to repair a city.
		//
		// None of these conditions can be affected by other rulesets.

		bool canRepair = false;
		wxInt32 trades = 0;

		// Condition 1.
		if(false == playerGameData<IntArray>(shSabotagedCities).empty())
		{
			// Condition 2.
			RULE.Decide(shLogicCanPurchaseRepairCity, input, output);
			wxASSERT(2 <= output.numItems());

			if(true == output.read<bool>())
			{
				trades = output.read<wxInt32>(1);
				canRepair = true;
			}
		}

		output = DataObject(canRepair, trades);
	}
};

IMPLEMENT_LOGIC(LogicCanRepairCity, CITIES)
