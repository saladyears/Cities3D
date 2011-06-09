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
#include "IRuleEngine.h"
#include "DummySelectionObject.h"
#include "SideObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRestartTurnDiplomat
// 
// Restarts the game from a player selecting an open road to remove.
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
class RuleRestartTurnDiplomat : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
        // The current player is now in a blocking action.
        RULE.Execute(shRuleBeginBlockingAction, DataObject(current()));

		RULE.Execute(shRuleDiplomatText, DataObject());

		Controller::get().Transmit(shEventControlsUI, DataObject(false));
		Controller::get().Transmit(shEventBuildUI, 
				DataObject(false, GetGame()));
	
		// Transmit the selections.
		Controller::get().Transmit(shEventSelectionObject, 
			SelectionObjectPtr(
			new DummySelectionObject(shRuleSelectDiplomatRoad)));

		// Get all of the side objects that can be removed.
		DataObject input(0), output;
		RULE.Decide(shLogicDiplomatPlaces, input, output);
		const PlayerGame::SideObjectArray& sides = 
			output.read<PlayerGame::SideObjectArray>();
		wxASSERT(false == sides.empty());
		
		// Now make them all selectable.
		RULE.Execute(shRuleResetTileFlags, DataObject(IGameObject::Dim));
		RULE.Execute(shRuleResetObjectFlags, DataObject(IGameObject::Dim));

		PlayerGame::SideObjectArray::const_iterator it, 
			itEnd = sides.end();
		for(it = sides.begin(); it != itEnd; ++it)
		{
			SideObjectPtr object = (*it);

			object->reset();
			object->set(IGameObject::Select);

			wxInt32 tile1 = object->tile1().first;
			wxInt32 tile2 = object->tile2().first;
			
			tile(tile1)->reset();

			if(-1 != tile2)
			{
				tile(tile2)->reset();
			}
		}
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RuleRestartTurnDiplomat, TurnDiplomat, 
						   RuleRestartGame, CITIES)
