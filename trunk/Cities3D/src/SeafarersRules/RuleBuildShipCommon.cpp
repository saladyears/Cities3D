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
#include "SideSelectionObject.h"
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
// Class: RuleBuildShipCommon
// 
// Mixin for handling a click on the ship <BuildUI> in the game state of Turn,
// allowing the player to place a new ship on the board.
//
// Derived From:
//     <Rule>
//     <IUndoable>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
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
class RuleBuildShipCommon 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
		const HashString& rule = object.read<HashString>();
		const HashString& state = object.read<HashString>(1);

		// Update the UI text.
		RULE.Execute(shRuleUpdateMessageUI, DataObject(stPlaceAShip, 
            stWaitingPlaceAShip));

		// Turn off everything.
		Controller::get().Transmit(shEventBuildUI, DataObject(false, 
			GetGame()));
		Controller::get().Transmit(shEventControlsUI, DataObject(false));

		// Get the set of all available ship positions for this player.
		DataObject input(0), output;
		RULE.Decide(shLogicCanPlaceShip, input, output);

		const TileSideSet &sides = output.read<TileSideSet>();
		wxASSERT(false == sides.empty());

		RULE.Execute(shRuleBuildShipCommonPlaces, DataObject(sides, rule));

		// This is a new game state.
		gameData<HashString>(shState) = state;
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

IMPLEMENT_RULE(RuleBuildShipCommon, SEAFARERS)
