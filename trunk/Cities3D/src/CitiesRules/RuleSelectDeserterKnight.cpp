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
#include "IRuleEngine.h"
#include "KnightObject.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleSelectDeserterKnight
// 
// Handles the player selecting which knight deserted.
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
class RuleSelectDeserterKnight : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
        // The current player has ended a blocking action.
        RULE.Execute(shRuleEndBlockingAction, DataObject(current()));

		// First, record all of the vital statistics of the knight, then
		// remove it from the board.

		// Get the id of the knight they picked.
		wxInt32 id = object.read<wxInt32>();

		// Find that ship in the player's list and remove it.
		PlayerGame::CornerObjectArray knights = 
			playerGame().getCornerObjects(shKnights);

		PlayerGame::CornerObjectArray::const_iterator it, 
			itEnd = knights.end();
		for(it = knights.begin(); it != itEnd; ++it)
		{
			CornerObjectPtr object = (*it);

			if(object->id() == id)
			{
				break;
			}
		}

		wxASSERT(itEnd != it);

		KnightObject* knight = static_cast<KnightObject*>(it->get());

		// Play the sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_SELECT_DESERTER_KNIGHT));

		// Save off the strength and active information.
		wxInt32 strength = knight->strength();
		gameData<wxInt32>(shDeserterKnightUpgrade) = knight->upgrade_turn();
		gameData<wxInt32>(shDeserterKnightActive) = knight->active();
		
		// Remove the object from the player.
		playerGame().removeCornerObject(shKnights, (*it));
		HashString type = HashString::Format(shStockKnightsString.c_str(), 
            strength);
		playerGameData<wxInt32>(type) += 1;
		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), current()));

		// Update the UI.
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stSelectsADeserter));

        // Update the board.
        Controller::get().Transmit(shEventDirty, 0); 

		// Now that that's all done, switch back to the current player and
		// see if they have any place to put a new knight.
		wxInt32 originalPlayer = gameData<wxInt32>(shOriginalPlayer);
		gameData<wxInt32>(shCurrentPlayer) = originalPlayer;

		bool canPlace = false;
		DataObject input(current()), output;
		RULE.Decide(shLogicCanPlaceKnight, input, output);

		// If they have positions to place, then see if they have the knights
		// in stock to place.
		typedef std::pair<wxInt32, wxInt32> TileCorner;
		typedef std::set<TileCorner> TileCornerSet;
		const TileCornerSet& corners = output.read<TileCornerSet>();

		if(false == corners.empty())
		{
			// See what actual strength of knight they can play, if any.
			while(strength > 0)
			{
				type = HashString::Format(shStockKnightsString.c_str(), 
                    strength);
				wxInt32 count = playerGameData<wxInt32>(type);
				if(0 < count)
				{
					break;
				}

				--strength;
			}
			
			// If they have an available knight, they can place it.
			if(0 < strength)
			{
				canPlace = true;
				gameData<wxInt32>(shDeserterKnightStrength) = strength;
			}
		}

		// If they can place it, start up that mode.
		if(true == canPlace)
		{
			gameData<HashString>(shState) = shTurnDeserterPlace;
			RULE.Execute(shRuleRestartTurnDeserterPlace, DataObject());
		}
		// Otherwise, we're done.
		else
		{
			Controller::get().Transmit(shEventKnightCounts, GetGame());

			RULE.Execute(shRulePlayProgressCardEnd, DataObject());
		}
	}
};

IMPLEMENT_RULE(RuleSelectDeserterKnight, CITIES)
