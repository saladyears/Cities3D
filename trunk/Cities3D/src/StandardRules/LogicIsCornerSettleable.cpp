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
// Class: LogicIsCornerSettleable
// 
// Determines if a settlement placement is valid placement is valid if the 
// player has two side objects connected from the placement and no blocking
// cities or settlements in between.
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
//
class LogicIsCornerSettleable : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		bool valid = false;

		// First, check for intervening cities or settlements.
		const CornerObject::TileCorner &thisTile = 
			input.read<CornerObject::TileCorner>();
		const HashString& type = input.read<HashString>(1);

		RULE.Decide(shLogicIsCornerBlocked, 
			DataObject(shSettlements, thisTile), output);
		if(false == output.read<bool>())
		{
			output.reset();
			RULE.Decide(shLogicIsCornerBlocked,
				DataObject(shCities, thisTile), output);

			if(false == output.read<bool>())
			{
				// Finally, check to make sure that the corner is reachable by
				// a ship.
				output.reset();
				RULE.Decide(shLogicIsCornerReachable,
					DataObject(type, thisTile), output);
				valid = output.read<bool>();
			}
		}

		output = DataObject(valid);
	}
};

IMPLEMENT_LOGIC(LogicIsCornerSettleable, STANDARD);
