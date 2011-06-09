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
#include "AggregatorObject.h"
#include "Controller.h"
#include "Utility.h"
#include "KnightObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePlaceIntrigueKnight
// 
// Moves an attacked knight on the board during a player's turn.
//
// Derived From:
//     <Rule>
//     <IUndoable>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
// Rules Called:
//     <RulePlaceKnight>
//     <RuleRestartTurn>
//
// Logic Called:
//     <LogicCostKnight>
//
class RulePlaceIntrigueKnight : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
        // The current player has ended a blocking action.
        RULE.Execute(shRuleEndBlockingAction, DataObject(current()));

		gameData<HashString>(shState) = shTurn;

		// Clear any dim tiles or objects.
		RULE.Execute(shRuleResetObjectFlags, DataObject());
		RULE.Execute(shRuleResetTileFlags, DataObject());

		// Placing a moved knight means we need to set it at its original
		// strength and upgrade turn so they cannot upgrade a knight twice
		// in the same turn after moving it.
		wxInt32 strength = gameData<wxInt32>(shMoveKnightStrength);
		wxInt32 upgrade = gameData<wxInt32>(shMoveKnightUpgradeTurn);
		wxBOOL active = gameData<wxInt32>(shMoveKnightActive);

		// Run the standard placement of the ship.
		RULE.Execute(shRulePlaceKnightCommon, DataObject(
			object.read<wxInt32>(), strength, upgrade));

		// If the knight was active, find it in the player's data and make it
		// so.
		if(TRUE == active)
		{
			wxInt32 tile, corner;
			boost::tie(tile, corner) = 
				Utility::decodeSel(object.read<wxInt32>());
			CornerObject::TileCorner tileCorner(tile, corner);
			
			PlayerGame::CornerObjectArray knights = 
				playerGame().getCornerObjects(shKnights);
			PlayerGame::CornerObjectArray::iterator it,
				itEnd = knights.end();

			for(it = knights.begin(); it != itEnd; ++it)
			{
				if(true == (*it)->onTileCorner(tileCorner))
				{
					KnightObject* thisKnight = 
						static_cast<KnightObject*>((*it).get());
					thisKnight->active(true, -1);
					break;
				}
			}
			wxASSERT(it != itEnd);
		}

		// Play the move sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_MOVE_KNIGHT));

		Controller::get().Transmit(shEventKnightCounts, GetGame());

        // Update the board.
        Controller::get().Transmit(shEventDirty, 0);

		// Update the UI.
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stWithdrawsAKnight));

		// Revert to the original player.
		wxInt32 original = gameData<wxInt32>(shOriginalPlayer);
		gameData<wxInt32>(shCurrentPlayer) = original;

		Controller::get().Transmit(shEventPlayerUI, DataObject(GetGame(), -2));

		// Longest Road Check.
		RULE.Execute(shRuleLongestRoadCheck, DataObject(true));

		// Back to turn actions.
		RULE.Execute(shRulePlayProgressCardEnd, DataObject());
	}
};

IMPLEMENT_RULE(RulePlaceIntrigueKnight, CITIES)
