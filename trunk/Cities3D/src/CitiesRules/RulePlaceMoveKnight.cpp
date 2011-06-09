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
#include "KnightObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePlaceMoveKnight
// 
// Moves a knight on the board during a player's turn.  *Undoable*.
//
// Derived From:
//     <Rule>
//     <IUndoable>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
// Rules Called:
//     <RulePlaceKnight>
//     <RuleRestartTurn>
//
// Logic Called:
//     <LogicCostKnight>
//
class RulePlaceMoveKnight 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
        // The current player has ended a blocking action.
        RULE.Execute(shRuleEndBlockingAction, DataObject(current()));

		gameData<HashString>(shState) = shTurn;

		// Clear any dim tiles or objects.
		RULE.Execute(shRuleResetObjectFlags, DataObject());
		RULE.Execute(shRuleResetTileFlags, DataObject());

		// Placing a moved knight means we need to set it at its original
		// strength and upgrade turn so they cannot upgrade a knight twice
		// in the same turn after moving it.
		wxInt32 strength = gameData<wxInt32>(shMoveKnightStrength);
		wxInt32 upgrade = gameData<wxInt32>(shMoveKnightUpgradeTurn);

		// Run the standard placement of the ship.
		RULE.Execute(shRulePlaceKnightCommon, DataObject(
			object.read<wxInt32>(), strength, upgrade));

		Controller::get().Transmit(shEventKnightCounts, GetGame());

		// Play the move sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_MOVE_KNIGHT));

		// Update the UI.
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stMovesAKnight));

		// Longest Road Check.
		RULE.Execute(shRuleLongestRoadCheck, DataObject(true));

		// Adjust stats.
		RULE.Execute(shRuleTakeAction, DataObject(current(), shMoveKnight));

		// Back to turn actions.
		RULE.Execute(shRuleRestartTurn, DataObject());
	}

	virtual void Unexecute(const DataObject &)
	{	
		// In this case, we need to also put the ship back where it originally
		// was instead of just undoing the movement destination.

		wxInt32 tile = gameData<wxInt32>(shMoveKnightTile);
		wxInt32 side = gameData<wxInt32>(shMoveKnightCorner);
		wxInt32 turn = gameData<wxInt32>(shMoveKnightTurn);

		wxInt32 strength = gameData<wxInt32>(shMoveKnightStrength);
		wxInt32 upgrade = gameData<wxInt32>(shMoveKnightUpgradeTurn);

		ColorType color = playerGame().color();

		KnightObject* knight = new KnightObject(current(), turn, color, 
			GetGame(), tile, side);

		// The knight had to be active.
		knight->active(true, -1);
		knight->strength(strength - 1);
		knight->upgrade(true, upgrade);

		Controller::get().Transmit(shEventKnightCounts, GetGame());

		CornerObjectPtr object(knight);
		playerGame().addCornerObject(shKnights, object);

		//send it to the view
		AggregatorObjectPtr aggregate(new AggregatorObject);
		aggregate->add(object);

		Controller::get().Transmit(shEventGameObjects, aggregate);

		// Rerun road length calculations.
		RULE.Execute(shRuleLongestRoadCheck, DataObject(true));

		// Update the UI.
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stUndoesAKnightMove));
	}

	virtual bool CanUndo() const
	{
		// The Longest Road check will automatically disallow undoes if it
		// alters anything.
		return true;
	}
};

IMPLEMENT_RULE(RulePlaceMoveKnight, CITIES)
