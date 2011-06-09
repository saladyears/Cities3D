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
#include "SideObject.h"
#include "Controller.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleSelectShipMove
// 
// Removes the selected ship and sets things up for the player to select where
// to move it to.
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
// Rules Called:
//     <RuleRestartTurnShipMovePlacement>
//
// Game State:
//     TurnShipMovePlacement
//
class RuleSelectShipMove : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		// The current player is now in a blocking action.
        RULE.Execute(shRuleBeginBlockingAction, DataObject(current()));

		// Get the id of the ship they picked.
		wxInt32 id = object.read<wxInt32>();

		// Find that ship in the player's list and remove it.
		PlayerGame::SideObjectArray ships = 
			playerGame().getSideObjects(shShips);

		PlayerGame::SideObjectArray::const_iterator it, 
			itEnd = ships.end();
		for(it = ships.begin(); it != itEnd; ++it)
		{
			SideObjectPtr object = (*it);

			if(object->id() == id)
			{
				break;
			}
		}

		wxASSERT(itEnd != it);

		SideObjectPtr ship = (*it);

		// Save off the original location and turn so that they can potentially
		// undo the move later.
		gameData<wxInt32>(shShipMoveTile) = ship->tile1().first;
		gameData<wxInt32>(shShipMoveSide) = ship->tile1().second;
		gameData<wxInt32>(shShipMoveTurn) = ship->turn();

		// Remove the object from the player.
		playerGame().removeSideObject(shShips, ship);

		// This is a new game state.
		gameData<HashString>(shState) = shTurnShipMovePlacement;

		// Just use the restart logic to set things up.
		RULE.Execute(shRuleRestartTurnShipMovePlacement, DataObject());
	}
};

IMPLEMENT_RULE(RuleSelectShipMove, SEAFARERS)
