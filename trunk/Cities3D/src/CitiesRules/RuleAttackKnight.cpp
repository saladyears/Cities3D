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
// Class: RuleAttackKnight
// 
// Handles a click on the Attack Knight <BuildUI>.
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
//     <RuleAttackKnight>
//
// Rules Called:
//     <RuleUpdateMessageUI>
//
// Logic Called:
//     <LogicCanAttackKnight>
//
// Selection Rule:
//     <RuleSelectAttackKnight>
//
// Game State:
//     TurnAttackKnightPlacement
//
// Transmits To:
//     eventBuildUI
//     eventSelectionObject
//
class RuleAttackKnight : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// Get the set of all available knight positions for this 
		// player.
		DataObject input(0), output;
		RULE.Decide(shLogicCanAttackKnight, input, output);

		PlayerGame::CornerObjectArray knights = 
			output.read<PlayerGame::CornerObjectArray>(2);
		wxASSERT(false == knights.empty());

		// Update the UI text.
		RULE.Execute(shRuleUpdateMessageUI, 
            DataObject(stSelectAKnightToAttackWith, stWaitingSelectAKnightToAttackWith));

		// This is a new game state.
		gameData<HashString>(shState) = shTurnSelectAttackKnight;

		// Turn off all BuildUIs.
		Controller::get().Transmit(shEventBuildUI, 
			DataObject(false, GetGame()));

		// Turn off all controls.
		Controller::get().Transmit(shEventControlsUI, DataObject(true));

		// Transmit the selections.
		Controller::get().Transmit(shEventSelectionObject, 
			SelectionObjectPtr(
			new DummySelectionObject(shRuleSelectAttackKnight)));

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
};

IMPLEMENT_RULE(RuleAttackKnight, CITIES)
