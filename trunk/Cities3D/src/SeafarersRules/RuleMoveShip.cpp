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
#include "HexObject.h"
#include "DummySelectionObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleMoveShip
// 
// Allows players to select a ship to move during their turn..
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
//     <RuleUpdateMessageUI>
//
// Logic Called:
//     <LogicCanMove>
//
// Selection Rule:
//     <RuleSelectShipMove>
//
// Game State:
//     TurnShipMove
//
// Transmits To:
//     eventBuildUI
//     eventSelectionObject
//
class RuleMoveShip : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// Grab all of the ships that can be moved.
		DataObject input(0), output;
		RULE.Decide(shLogicCanMoveShip, input, output);
		wxASSERT(2 <= output.numItems());

		// Send up a dummy selection object to prepare things.
		DummySelectionObject *selection = new DummySelectionObject(
			shRuleSelectShipMove);

		Controller::get().Transmit(shEventSelectionObject, 
			SelectionObjectPtr(selection));

		// Make sure eveything is hidden.
		Controller::get().Transmit(shEventBuildUI, 
			DataObject(false, GetGame()));
		Controller::get().Transmit(shEventControlsUI, DataObject(false));

		RULE.Execute(shRuleResetTileFlags, DataObject(IGameObject::Dim));
		RULE.Execute(shRuleResetObjectFlags, DataObject(IGameObject::Dim));

		PlayerGame::SideObjectArray ships = 
			output.read<PlayerGame::SideObjectArray>(2);

		// Now that we have them all, make them selectable.
		PlayerGame::SideObjectArray::iterator it, itEnd = ships.end();
		for(it = ships.begin(); it != itEnd; ++it)
		{
			SideObjectPtr object = (*it);

			object->reset();
			object->set(IGameObject::Select);

			wxInt32 tile1 = object->tile1().first;
			wxInt32 side1 = object->tile1().second;

			wxInt32 tile2 = tile<IntArray>(shSides, tile1)[side1];

			tile(tile1)->reset();

			if(-1 != tile2)
			{
				tile(tile2)->reset();
			}
		}

		// Update the UI text.
		RULE.Execute(shRuleUpdateMessageUI, 
			DataObject(stSelectAShipToMove, stWaitingSelectAShipToMove));

		// This is a new game state.
		gameData<HashString>(shState) = shTurnShipMove;

		// In addition, turn on the pirate tile with the angry red outline.
		const HexObjectPtr& pirate = GetGame()->getHexObject(shPirate);
		wxASSERT(pirate);

		wxInt32 pirateTile = pirate->tile();
		if(-1 != pirateTile)
		{
			tile(pirateTile)->reset();
			tile(pirateTile)->set(TileObject::OutlineRed);
		}

		// The user performed a game action.
		RULE.Execute(shRulePerformGameAction, DataObject());
	}
};

IMPLEMENT_RULE(RuleMoveShip, SEAFARERS)
