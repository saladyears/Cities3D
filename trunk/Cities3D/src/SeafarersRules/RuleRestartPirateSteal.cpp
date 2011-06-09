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
#include "Controller.h"
#include "DummySelectionObject.h"
#include "SideObject.h"
#include "HexObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRestartPirateSteal
// 
// Restarts the game from a player selecting a player to rob after playing the
// pirate.
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
//     <RuleRestartGame>
//
class RuleRestartPirateSteal : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// The current player is now in a blocking action.
        RULE.Execute(shRuleBeginBlockingAction, DataObject(current()));

		RULE.Execute(shRuleUpdateMessageUI, DataObject(
            stChooseAPlayerToStealFrom, stWaitingToSteal));

		// Grab the current pirate tile.
		HexObjectPtr pirate = GetGame()->getHexObject(shPirate);
		wxASSERT(pirate);
		wxInt32 pirateTile = pirate->tile();

		//set the dummy selection
		DummySelectionObject *pSelection = new DummySelectionObject(
			shRuleSelectPirateSteal);

		//send the dummy selection object up
		Controller::get().Transmit(shEventSelectionObject, 
			SelectionObjectPtr(pSelection));

		// Make sure eveything is hidden.
		Controller::get().Transmit(shEventBuildUI, 
			DataObject(false, GetGame()));
		Controller::get().Transmit(shEventControlsUI, DataObject(false));

		RULE.Execute(shRuleResetTileFlags, DataObject(IGameObject::Dim));
		RULE.Execute(shRuleResetObjectFlags, DataObject(IGameObject::Dim));

		// Turn on the pirate tile.
		tile(pirateTile)->reset();

		// Get all of the objects that may be stolen from.
		DataObject output;
		RULE.Decide(shLogicPirateStealPlayers, DataObject(pirateTile), output);
		const PlayerGame::SideObjectArray& objects =
			output.read<PlayerGame::SideObjectArray>(1);

		// Turn on selection for these objects and undim the tiles around them.
		PlayerGame::SideObjectArray::const_iterator it, 
			itEnd = objects.end();
		for(it = objects.begin(); it != itEnd; ++it)
		{
			(*it)->reset();
			(*it)->set(IGameObject::Select);
		}

		// Turn on selection for the pirate.
		pirate->reset();
		pirate->set(IGameObject::Select);
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RuleRestartPirateSteal, PirateSteal, 
						   RuleRestartGame, SEAFARERS)
