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
#include "CornerSelectionObject.h"
#include "Controller.h"
#include "Utility.h"
#include "UtilityGL.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleBuildSettlementTurn
// 
// Mixin for handling a click on the settlement <BuildUI> in the game state of 
// Turn, allowing the player to place a new settlement on the board.
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
//     <RuleBuildSettlement>
//
// Rules Called:
//     <RuleUpdateMessageUI>
//
// Logic Called:
//     <LogicCanPlaceSettlement>
//
// Selection Rule:
//     <RulePlaceSettlementTurn>
//
// Game State:
//     TurnSettlementPlacement
//
// Transmits To:
//     eventBuildUI
//     eventSelectionObject
//
class RuleBuildSettlementTurn 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &)
	{
		// First, make sure we have all the resources needed to do this.
		RULE.Execute(shRuleExecuteAutoTrade, DataObject(
			shRuleBuildSettlementTurn, shLogicCostSettlement));

		// Check to see if manual auto-trade is turned on.
		if(false == gameData<HashString>(shAutoTradeRule).empty())
		{
			return;
		}

		// Next, get the set of all available settlement positions for this 
		// player.
		DataObject input(0), output;
		RULE.Decide(shLogicCanPlaceSettlement, input, output);

		const TileCornerSet &corners = output.read<TileCornerSet>();

		wxASSERT(false == corners.empty());

		// Create a CornerSelectionObject that holds all of the road placement 
		// possibilities.
		ColorType color = playerGame().color();
		CornerSelectionObject *selections = new CornerSelectionObject(
			shRulePlaceSettlementTurn, color);

		TileCornerSet::const_iterator it, itEnd = corners.end();
		for(it = corners.begin(); it != itEnd; ++it)
		{
			Vector coords = tile(it->first)->coords();
			UtilityGL::cornerCoords(coords, it->second);
			selections->add(CornerSelectionObject::CornerSelectionPair(
				Utility::encodeSel(it->first, it->second), coords));
		}

		// Update the UI text.
		RULE.Execute(shRuleUpdateMessageUI, 
			DataObject(stPlaceASettlement, stWaitingPlaceASettlement));

		// This is a new game state.
		gameData<HashString>(shState) = shTurnSettlementPlacement;

		// Turn off all BuildUIs.
		Controller::get().Transmit(shEventBuildUI, 
			DataObject(false, GetGame()));

		// Turn off all controls.
		Controller::get().Transmit(shEventControlsUI, DataObject(true));

		// Transmit the selections.
		Controller::get().Transmit(shEventSelectionObject, 
			SelectionObjectPtr(selections));

		// For visual help, dim all tiles except those on which the settlements
		// can be placed.
		RULE.Execute(shRuleResetTileFlags, DataObject(IGameObject::Dim));
		for(it = corners.begin(); it != itEnd; ++it)
		{
			wxInt32 corner = it->second;

			wxInt32 tile1 = it->first;
			wxInt32 tile2 = tile<IntArray>(shSides, tile1)[corner];
			wxInt32 tile3 = tile<IntArray>(shSides, tile1)[(corner + 5) % 6];

			tile(tile1)->reset();

			if(-1 != tile2)
			{
				tile(tile2)->reset();
			}

			if(-1 != tile3)
			{
				tile(tile3)->reset();
			}
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
	typedef std::pair<wxInt32, wxInt32> TileCorner;
	typedef std::set<TileCorner> TileCornerSet;
};

IMPLEMENT_RULE_KEYED_MIXIN(RuleBuildSettlementTurn, Turn, RuleBuildSettlement, STANDARD)
