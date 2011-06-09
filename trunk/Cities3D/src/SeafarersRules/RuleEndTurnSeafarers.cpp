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
#include "Rule.h"
#include "RuleSetDatabase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleEndTurnSeafarers
// 
// Extension to RuleEndTurn for ship movement handling.
//
// Derived From:
//     <Rule>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <Seafarers>
//
// Mixin To:
//     <RuleEndTurn>
//
class RuleEndTurnSeafarers : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// They are free to move another ship next turn.
		playerGameData<wxInt32>(shMovedShip) = 0;

		// Get rid of any ship move undo information.
		eraseGameData<wxInt32>(shShipMoveTile);
		eraseGameData<wxInt32>(shShipMoveSide);
		eraseGameData<wxInt32>(shShipMoveTurn);
	}
};

IMPLEMENT_RULE_MIXIN(RuleEndTurnSeafarers, RuleEndTurn, SEAFARERS)
