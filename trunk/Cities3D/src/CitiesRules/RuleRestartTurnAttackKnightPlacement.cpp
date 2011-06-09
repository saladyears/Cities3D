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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRestartTurnAttackKnightPlacement
// 
// Restarts the game from a player selecting a location to move their knight
// during their turn.
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
// Rules Called:
//     <RuleBuildKnight>
//
// Selection Rule:
//     <RuleSelectAttackKnight>
//
// Transmits To:
//     eventBuildUI
//     eventSelectionObject
//
class RuleRestartTurnAttackKnightPlacement : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
        // The current player is now in a blocking action.
        RULE.Execute(shRuleBeginBlockingAction, DataObject(current()));

		wxInt32 curPlayer = current();

		// Get all of the available placements.
		wxInt32 thisTile = gameData<wxInt32>(shAttackKnightTile);
		wxInt32 thisSide = gameData<wxInt32>(shAttackKnightCorner);
		wxInt32 strength = gameData<wxInt32>(shAttackKnightStrength);

		// Get all of the available corners that can be attacked.
		DataObject input(TileCorner(thisTile, thisSide), curPlayer, strength), 
			output;
		RULE.Decide(shLogicKnightMovePlaces, input, output);

		TileCornerSet corners = output.read<TileCornerSet>();
		wxASSERT(false == corners.empty());

		// Go through everyone else's knights and find the ones on these
		// corners.
		PlayerGame::CornerObjectArray knights;
		{
			TileCornerSet::const_iterator it, itEnd = corners.end();
			for(it = corners.begin(); it != itEnd; ++it)
			{
				const TileCorner& tileCorner = (*it);

				for(wxInt32 i = 0; i < numPlayers(); ++i)
				{
					if(i == curPlayer)
					{
						continue;
					}

					bool found = false;

					const PlayerGame::CornerObjectArray& objects =
						playerGame(i).getCornerObjects(shKnights);
					PlayerGame::CornerObjectArray::const_iterator itKn,
						itKnEnd = objects.end();
					for(itKn = objects.begin(); itKn != itKnEnd; ++itKn)
					{
						const CornerObjectPtr& knight = (*itKn);
						if(true == knight->onTileCorner(tileCorner))
						{
							knights.push_back(knight);
							found = true;
							break;
						}	
					}

					if(true == found)
					{
						break;
					}
				}
			}
		}

		// Transmit the selections.
		Controller::get().Transmit(shEventSelectionObject, 
			SelectionObjectPtr(
			new DummySelectionObject(shRuleAttackOpponentKnight)));

		// Now that we have all of the knights, prepare them for selection.
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

		// Make sure eveything is hidden.
		Controller::get().Transmit(shEventBuildUI, 
			DataObject(false, GetGame()));
		Controller::get().Transmit(shEventControlsUI, DataObject(false));

		// Update the UI text.
		RULE.Execute(shRuleUpdateMessageUI, 
			DataObject(stAttackAnOpponentsKnight, stWaitingAttackAnOpponentsKnight));
	}

private:
	typedef std::pair<wxInt32, wxInt32> TileCorner;
	typedef std::set<TileCorner> TileCornerSet;
};

IMPLEMENT_RULE_KEYED_MIXIN(RuleRestartTurnAttackKnightPlacement, 
						   TurnAttackKnightPlacement, RuleRestartGame, CITIES)
