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
#include "CornerObject.h"
#include "Controller.h"
#include "KnightObject.h"
#include "IRuleEngine.h"
#include "Utility.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleSelectIntrigueKnight
// 
// Removes the displaced knight and either prepares for the player moving it
// elsewhere or removes it from the board if there is no place to move it.
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
// Rules Called:
//     <RuleRestartTurnAttackKnightPlacement>
//
// Game State:
//     TurnAttackKnightPlacement
//
class RuleSelectIntrigueKnight : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
        // The current player has ended a blocking action.
        RULE.Execute(shRuleEndBlockingAction, DataObject(current()));

		// Get the id of the knight they attacked.
		wxInt32 id = object.read<wxInt32>();

		wxInt32 curPlayer = current();
		wxInt32 attackPlayer = -1;

		CornerObjectPtr knight;

		// Find out which knight got attacked.
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			if(i == curPlayer)
			{
				continue;
			}

			bool found = false;

			const PlayerGame::CornerObjectArray& objects =
				playerGame(i).getCornerObjects(shKnights);
			PlayerGame::CornerObjectArray::const_iterator it,
				itEnd = objects.end();
			for(it = objects.begin(); it != itEnd; ++it)
			{
				if(id == (*it)->id())
				{
					attackPlayer = i;
					knight = (*it);
					found = true;
					break;
				}	
			}

			if(true == found)
			{
				break;
			}
		}
		wxASSERT(knight);
		wxASSERT(-1 != attackPlayer);

		// Update the UI.
		wxString str = wxString::Format(stDisplacesAKnightOf.c_str(),
			swStringFormat.c_str(), playerGame(attackPlayer).player().Name().c_str());
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(str));

		// Now see if it's even possible for the player who's knight was 
		// attacked to move it.
		DataObject input(knight->tile1(), attackPlayer), output;
		RULE.Decide(shLogicKnightMovePlaces, input, output);

		const TileCornerSet& corners = output.read<TileCornerSet>();

		const KnightObject* thisKnight = static_cast<const KnightObject*>(
			knight.get());

		// Play the sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_SELECT_INTRIGUE_KNIGHT));

		// If they have no options, the knight is destroyed.
		if(true == corners.empty())
		{
			gameData<HashString>(shState) = shTurn;

			// Update the UI.
			RULE.Execute(shRuleUpdateNetworkUI, DataObject(stLosesTheKnight, 
                attackPlayer));

			// Add the knight back to their inventory.
			HashString stock = HashString::Format(shStockKnightsString.c_str(), 
				thisKnight->strength());
			playerGameData<wxInt32>(stock, attackPlayer)++;

			Controller::get().Transmit(shEventPlayerUI, 
				DataObject(GetGame(), attackPlayer));
			Controller::get().Transmit(shEventKnightCounts, GetGame());

			RULE.Execute(shRulePlaySound, DataObject(SOUND_DESTROY_KNIGHT));

			// Back to turn actions.
			RULE.Execute(shRulePlayProgressCardEnd, DataObject());
		}
		else
		{
			gameData<HashString>(shState) = shTurnIntriguePlace;

			// Store the current player becuase we are going to change it to
			// the player who is being attacked.
			gameData<wxInt32>(shOriginalPlayer) = curPlayer;
			gameData<wxInt32>(shCurrentPlayer) = attackPlayer;

			// Save off the data and turn so it can be recreated
			// after the move.
			gameData<wxInt32>(shMoveKnightTile) = thisKnight->tile1().first;
			gameData<wxInt32>(shMoveKnightCorner) = thisKnight->tile1().second;
			gameData<wxInt32>(shMoveKnightTurn) = thisKnight->turn();
			gameData<wxInt32>(shMoveKnightActive) = 
				thisKnight->active() ? TRUE : FALSE;
			gameData<wxInt32>(shMoveKnightStrength) = thisKnight->strength();
			gameData<wxInt32>(shMoveKnightUpgradeTurn) = 
				thisKnight->upgrade_turn();

			// Just run a restart.
			RULE.Execute(shRuleRestartTurnIntriguePlace, DataObject());
		}

		// Remove this knight from the player's list.
		playerGame(attackPlayer).removeCornerObject(shKnights, knight);

        // Update the board.
        Controller::get().Transmit(shEventDirty, 0);
	}

private:
	typedef std::pair<wxInt32, wxInt32> TileCorner;
	typedef std::set<TileCorner> TileCornerSet;
};

IMPLEMENT_RULE(RuleSelectIntrigueKnight, CITIES)
