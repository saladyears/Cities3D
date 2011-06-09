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
#include "Controller.h"
#include "CornerObject.h"
#include "DummySelectionObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleBuildCityTurn
// 
// Mixin for handling a click on the road <BuildUI> in the game state of Turn,
// allowing the player to place a new road on the board.
//
// Derived From:
//     <Rule>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// Mixin To:
//     <RuleBuildCity>
//
// Rules Called:
//     <RuleUpdateMessageUI>
//
// Logic Called:
//     <LogicCanPlaceCity>
//
// Selection Rule:
//     <RulePlaceCityTurn>
//
// Game State:
//     TurnCityPlacement
//
// Transmits To:
//     eventBuildUI
//     eventSelectionObject
//
class RuleBuildCityTurn 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &)
	{
		// First, make sure we have all the resources needed to do this.
		RULE.Execute(shRuleExecuteAutoTrade, DataObject(
			shRuleBuildCityTurn, shLogicCostCity));

		// Check to see if manual auto-trade is turned on.
		if(false == gameData<HashString>(shAutoTradeRule).empty())
		{
			return;
		}

		// Create a selection object with the rule to use.
		SelectionObjectPtr selection(new DummySelectionObject(
			shRulePlaceCityTurn));
		
		// Transmit the selection.
		Controller::get().Transmit(shEventSelectionObject, selection);

		// Turn on selection for all settlements for this player.
		const PlayerGame::CornerObjectArray& settlements =
			playerGame().getCornerObjects(shSettlements);

		PlayerGame::CornerObjectArray::const_iterator it,
			itEnd = settlements.end();
		for(it = settlements.begin(); it != itEnd; ++it)
		{
			(*it)->set(IGameObject::Select);
		}

		// Update the UI text.
		RULE.Execute(shRuleUpdateMessageUI, 
			DataObject(stPlaceACity, stWaitingPlaceACity));

		// This is a new game state.
		gameData<HashString>(shState) = shTurnCityPlacement;

		// Turn off all BuildUIs.
		Controller::get().Transmit(shEventBuildUI, 
			DataObject(false, GetGame()));

		// Turn off all controls.
		Controller::get().Transmit(shEventControlsUI, DataObject(true));

		// For visual help, dim all tiles except those on which the settlements
		// can be placed.
		RULE.Execute(shRuleResetTileFlags, DataObject(IGameObject::Dim));
		for(it = settlements.begin(); it != itEnd; ++it)
		{
			// A settlement must always be on 3 valid corners.
			tile((*it)->tile1().first)->reset();
			tile((*it)->tile2().first)->reset();
			tile((*it)->tile3().first)->reset();
		}

        // The user performed a game action.
        RULE.Execute(shRulePerformGameAction, DataObject());
	}

	virtual void Unexecute(const DataObject&)
	{
		// Reset the selection.
		Controller::get().Transmit(shEventSelectionObject, 
			SelectionObjectPtr());

		gameData<HashString>(shState) = shTurn;
		RULE.Execute(shRuleRestartTurn, DataObject());
	}

	virtual bool CanUndo() const
	{
		// We need to support undo for backing out of an auto-trade.
		return true;
	}

private:
	typedef std::pair<wxInt32, wxInt32> TileSide;
	typedef std::set<TileSide> TileSideSet;
};

IMPLEMENT_RULE_KEYED_MIXIN(RuleBuildCityTurn, Turn, RuleBuildCity, STANDARD)
