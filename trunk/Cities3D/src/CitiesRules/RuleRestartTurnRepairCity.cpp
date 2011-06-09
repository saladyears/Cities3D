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
#include "IRuleEngine.h"
#include "DummySelectionObject.h"
#include "CornerObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRestartTurnRepairCity
// 
// Restarts the game from a player selecting a city to repair.
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
//     <RuleRestartGame>
//
class RuleRestartTurnRepairCity 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &)
	{
		// First, make sure we have all the resources needed to do this.
		RULE.Execute(shRuleExecuteAutoTrade, DataObject(
			HashString(shRuleRestartTurnRepairCity), shLogicCostRepairCity));

		// Check to see if manual auto-trade is turned on.
		if(false == gameData<HashString>(shAutoTradeRule).empty())
		{
			return;
		}

		RULE.Execute(shRuleUpdateMessageUI, DataObject(stRepairACity, 
            stWaitingRepairACity));

		Controller::get().Transmit(shEventControlsUI, DataObject(false));
		Controller::get().Transmit(shEventBuildUI, 
				DataObject(false, GetGame()));
	
		// Transmit the selections.
		Controller::get().Transmit(shEventSelectionObject, 
			SelectionObjectPtr(
			new DummySelectionObject(shRuleSelectRepairCity)));

		// Go through all their cities and use the ones that are in their
		// sabotage list.
		const IntArray& sabotaged = 
			playerGameData<IntArray>(shSabotagedCities);
		const PlayerGame::CornerObjectArray &cities = 
			playerGame().getCornerObjects(shCities);

		// Now make them all selectable.
		RULE.Execute(shRuleResetTileFlags, DataObject(IGameObject::Dim));
		RULE.Execute(shRuleResetObjectFlags, DataObject(IGameObject::Dim));

		PlayerGame::CornerObjectArray::const_iterator it, 
			itEnd = cities.end();
		for(it = cities.begin(); it != itEnd; ++it)
		{
			CornerObjectPtr object = (*it);
			wxInt32 id = object->id();

			if(sabotaged.end() != std::find(sabotaged.begin(), 
				sabotaged.end(), id))
			{
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

IMPLEMENT_RULE_KEYED_MIXIN(RuleRestartTurnRepairCity, TurnRepairCity, 
						   RuleRestartGame, CITIES)
