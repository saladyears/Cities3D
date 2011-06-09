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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleBuildShipTurn
// 
// Mixin for handling a click on the ship <BuildUI> in the game state of Turn,
// allowing the player to place a new ship on the board.
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
//     <RuleBuildShip>
//
// Rules Called:
//     <RuleUpdateMessageUI>
//
// Logic Called:
//     <LogicCanPlaceShip>
//
// Selection Rule:
//     <RulePlaceShipTurn>
//
// Game State:
//     TurnShipPlacement
//
// Transmits To:
//     eventBuildUI
//     eventSelectionObject
//
class RuleBuildShipTurn 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &)
	{
		// First, make sure we have all the resources needed to do this.
		RULE.Execute(shRuleExecuteAutoTrade, DataObject(shRuleBuildShipTurn, 
            shLogicCostShip));

		// Check to see if manual auto-trade is turned on.
		if(false == gameData<HashString>(shAutoTradeRule).empty())
		{
			return;
		}

		RULE.Execute(shRuleBuildShipCommon, DataObject(shRulePlaceShipTurn, 
            shTurnShipPlacement));

		// The user performed a game action.
		RULE.Execute(shRulePerformGameAction, DataObject());
	}

	virtual void Unexecute(const DataObject&)
	{
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

IMPLEMENT_RULE_KEYED_MIXIN(RuleBuildShipTurn, Turn, RuleBuildShip, SEAFARERS)
