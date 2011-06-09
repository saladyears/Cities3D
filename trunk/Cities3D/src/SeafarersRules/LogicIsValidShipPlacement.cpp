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
#include "SideObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicIsValidShipPlacement
// 
// Determines if a ship placement is valid in a Seafarers game.
//
// Derived From:
//     <Logic>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <Seafarers>
//
// Mixin To:
//     <LogicIsValidShipPlacement>
//
// Logic Called:
//     <LogicIsValidSidePlacement>
//
class LogicIsValidShipPlacement : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		const wxInt32 index = input.read<wxInt32>();
		const wxInt32 side = input.read<wxInt32>(1);

		RULE.Decide(shLogicIsValidSidePlacement, 
			DataObject(shShips, index, side), output);
	}
};

IMPLEMENT_LOGIC(LogicIsValidShipPlacement, SEAFARERS);
