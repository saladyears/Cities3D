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
// Class: LogicCanPurchaseUpgrade
// 
// Determines if a player has the resources required to purchase a city
// upgrade of the given kind.
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
// Logic Called:
//     <LogicCostUpgrade>
//     <LogicCanPurchase>
//
class LogicCanPurchaseUpgrade : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		const HashString& type = input.read<HashString>();
		HashString logic = HashString::Format(shLogicCostUpgradeString.c_str(), 
            type.c_str());

		// Get how much the settlement costs.
		DataObject thisOutput;
		RULE.Decide(logic, DataObject(0), thisOutput);
		
		// See if it can be bought.
		RULE.Decide(shLogicCanPurchase, thisOutput, output);
	}
};

IMPLEMENT_LOGIC(LogicCanPurchaseUpgrade, CITIES)
