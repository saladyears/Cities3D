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
#include "GameEngine.h"
#include "RuleExtensionMixin.h"
#include "IUndoable.h"
#include "KnightObject.h"
#include "Controller.h"
#include "Utility.h"
#include "AnimationPlacement.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleSelectActivateKnight
// 
// Activates the knight on the board at the given tile and corner for the 
// current player.  *Undoable*.
//
// Derived From:
//     <Rule>,
//     <IUndoable>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
// Rules Called:
//     <RuleUpdateNetworkUI>
//     <RuleAnimateSelectment>
//
// Transmits To:
//     eventGameObjects
//
class RuleSelectActivateKnight 
	: public Rule
	, public IUndoable
	, public RuleExtensionMixin
{
public:
	virtual void Execute(const DataObject &object)
	{
		gameData<HashString>(shState) = shTurn;

		// Clear any dim tiles or objects.
		RULE.Execute(shRuleResetTileFlags, DataObject());
		RULE.Execute(shRuleResetObjectFlags, DataObject());

		wxASSERT(1 <= object.numItems());

		// Update the UI.
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stActivatesAKnight));

		//first, determine the tile and corner being built on
		wxInt32 tile, corner;
		boost::tie(tile, corner) = Utility::decodeSel(object.read<wxInt32>());

		PlayerGame& playergame(playerGame());

		// Now find the knight and activate it.
		PlayerGame::CornerObjectArray knights = 
			playergame.getCornerObjects(shKnights);
		CornerObjectPtr knight;
		PlayerGame::CornerObjectArray::iterator it, itEnd = knights.end();
		for(it = knights.begin(); it != itEnd; ++it)
		{
			if(true == (*it)->onTileCorner(
				CornerObject::TileCorner(tile, corner)))
			{
				knight = (*it);
				break;
			}
		}
		wxASSERT(knight);

		wxInt32 turn = gameData<wxInt32>(shTurn);

		// Turn on the knight.
		KnightObject* thisKnight = static_cast<KnightObject*>(knight.get());
		thisKnight->active(true, turn);

        Controller::get().Transmit(shEventDirty, 0);

		Controller::get().Transmit(shEventKnightCounts, GetGame());

		// Play the activate sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_ACTIVATE_KNIGHT));

		// Run an animation if needed.
		RULE.Execute(shRuleAnimatePlacement, DataObject(playergame.player(),
			GameObjectPtr(knight), playergame.color()));

		// Execute any extensions to walls placement.
		ExecuteExtensions(object);

		// Adjust the resources used.
		DataObject input(0), output;
		RULE.Decide(shLogicCostActivateKnight, input, output);
		RULE.Execute(shRulePurchase, output);

		// Adjust stats.
		RULE.Execute(shRulePurchaseItem, DataObject(current(), shActivate));

		// Back to turn actions.
		RULE.Execute(shRuleRestartTurn, DataObject());
	}

	virtual void Unexecute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		//first, determine the tile and corner being built on
		wxInt32 tile, corner;
		boost::tie(tile, corner) = Utility::decodeSel(object.read<wxInt32>());

		// Now find the knight and activate it.
		PlayerGame::CornerObjectArray knights = 
			playerGame().getCornerObjects(shKnights);
		CornerObjectPtr knight;
		PlayerGame::CornerObjectArray::iterator it, itEnd = knights.end();
		for(it = knights.begin(); it != itEnd; ++it)
		{
			if(true == (*it)->onTileCorner(
				CornerObject::TileCorner(tile, corner)))
			{
				knight = (*it);
				break;
			}
		}
		wxASSERT(knight);

		// Turn off the knight.
		KnightObject* thisKnight = static_cast<KnightObject*>(knight.get());
		thisKnight->active(false, -1);

		Controller::get().Transmit(shEventKnightCounts, GetGame());

		// Update the UI.
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stUndoesAKnightActivate));
	}

	virtual bool CanUndo() const
	{
		return true;
	}
};

IMPLEMENT_RULE(RuleSelectActivateKnight, CITIES)
