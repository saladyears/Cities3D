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
#include "IUndoable.h"
#include "RuleSetDatabase.h"
#include "AggregatorObject.h"
#include "Controller.h"
#include "ShipObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePlaceShipMove
// 
// Moves a ship on the board during a player's turn.  *Undoable*.
//
// Derived From:
//     <Rule>
//     <IUndoable>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <Seafarers>
//
// Rules Called:
//     <RulePlaceShip>
//     <RuleRestartTurn>
//
// Logic Called:
//     <LogicCostShip>
//
class RulePlaceShipMove 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
		gameData<HashString>(shState) = shTurn;

		// Clear any dim tiles or objects.
		RULE.Execute(shRuleResetObjectFlags, DataObject());
		RULE.Execute(shRuleResetTileFlags, DataObject());

		// Run the standard placement of the ship.
		RULE.Execute(shRulePlaceShipCommon, object);

		// Mark that they have moved a ship.
		playerGameData<wxInt32>(shMovedShip) = TRUE;

		// Play the correct sound
		RULE.Execute(shRulePlaySound, DataObject(SOUND_MOVE_SHIP));

		// Update the UI.
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stMovesAShip));

		// Longest Road Check.
		RULE.Execute(shRuleLongestRoadCheck, DataObject(true));

		// Adjust stats.
		RULE.Execute(shRuleTakeAction, DataObject(current(), shMoveShip));

		// Back to turn actions.
		RULE.Execute(shRuleRestartTurn, DataObject());
	}

	virtual void Unexecute(const DataObject &)
	{
		playerGameData<wxInt32>(shMovedShip) = FALSE;
		
		// In this case, we need to also put the ship back where it originally
		// was instead of just undoing the movement destination.

		wxInt32 tile = gameData<wxInt32>(shShipMoveTile);
		wxInt32 side = gameData<wxInt32>(shShipMoveSide);
		wxInt32 turn = gameData<wxInt32>(shShipMoveTurn);

		ColorType color = playerGame().color();

		SideObjectPtr ship(new ShipObject(current(), turn, color, GetGame(), 
			tile, side));
		playerGame().addSideObject(shShips, ship);

		//send it to the view
		AggregatorObjectPtr aggregate(new AggregatorObject);
		aggregate->add(ship);

		Controller::get().Transmit(shEventGameObjects, aggregate);

		// Rerun road length calculations.
		RULE.Execute(shRuleLongestRoadCheck, DataObject(true));

		// Update the UI.
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stUndoesAShipMove));
	}

	virtual bool CanUndo() const
	{
		// The Longest Road check will automatically disallow undoes if it
		// alters anything.
		return true;
	}
};

IMPLEMENT_RULE(RulePlaceShipMove, SEAFARERS)
