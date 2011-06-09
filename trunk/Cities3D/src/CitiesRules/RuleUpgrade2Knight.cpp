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
#include "DummySelectionObject.h"
#include "CornerObject.h"
#include "Controller.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleUpgrade2Knight
// 
// Handles a click on the upgrade to strong knight BuildUI.
//
// Derived From:
//     <Rule>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
// Mixin To:
//     <RuleUpgrade2Knight>
//
// Rules Called:
//     <RuleUpdateMessageUI>
//
// Logic Called:
//     <LogicCanPlace2Knight>
//
// Selection Rule:
//     <RuleSelect2Knight>
//
// Game State:
//     Turn2KnightPlacement
//
// Transmits To:
//     eventBuildUI
//     eventSelectionObject
//
class RuleUpgrade2Knight 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &)
	{
		// First, make sure we have all the resources needed to do this.
		RULE.Execute(shRuleExecuteAutoTrade, DataObject(shRuleUpgrade2Knight, 
            shLogicCostKnightUpgrade));

		// Check to see if manual auto-trade is turned on.
		if(false == gameData<HashString>(shAutoTradeRule).empty())
		{
			return;
		}

		// Next, get the set of all available knight positions for this 
		// player.
		DataObject input(0), output;
		RULE.Decide(shLogicCanPlace2Knight, input, output);

		PlayerGame::CornerObjectArray knights = 
			output.read<PlayerGame::CornerObjectArray>();
		wxASSERT(false == knights.empty());

		// Update the UI text.
		RULE.Execute(shRuleUpdateMessageUI, DataObject(stUpgradeToAStrongKnight, 
            stWaitingUpgradeToAStrongKnight));

		// This is a new game state.
		gameData<HashString>(shState) = shTurn2Knight;

		// Turn off all BuildUIs.
		Controller::get().Transmit(shEventBuildUI, 
			DataObject(false, GetGame()));

		// Turn off all controls.
		Controller::get().Transmit(shEventControlsUI, DataObject(true));

		// Transmit the selections.
		Controller::get().Transmit(shEventSelectionObject, 
			SelectionObjectPtr(
			new DummySelectionObject(shRuleSelect2Knight)));

		// For visual help, dim all tiles except those on which the walls
		// can be placed.
		RULE.Execute(shRuleResetTileFlags, DataObject(IGameObject::Dim));
		RULE.Execute(shRuleResetObjectFlags, DataObject(IGameObject::Dim));

		PlayerGame::CornerObjectArray::iterator it, itEnd = knights.end();
		for(it = knights.begin(); it != itEnd; ++it)
		{
			CornerObjectPtr object = (*it);

			object->reset();
			object->set(IGameObject::Select);

			wxInt32 tile1 = object->tile1().first;
			wxInt32 tile2 = object->tile2().first;
			wxInt32 tile3 = object->tile3().first;

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
};

IMPLEMENT_RULE(RuleUpgrade2Knight, CITIES)
