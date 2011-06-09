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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleSelectExpelKnight
// 
// Deactivates the selected knight and moves to expel criminals.
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
//     <RuleRestartTurnExpelKnightPlacement>
//
// Game State:
//     TurnExpelKnightPlacement
//
class RuleSelectExpelKnight : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
        // The current player is now in a blocking action.
        RULE.Execute(shRuleBeginBlockingAction, DataObject(current()));

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

		KnightObject* knight = static_cast<KnightObject*>((*it).get());

		// This knight is no longer active.
		knight->active(false, -1);
		Controller::get().Transmit(shEventKnightCounts, GetGame());

        Controller::get().Transmit(shEventDirty, 0);

		// New state.
		gameData<HashString>(shState) = shExecuteExpel;

		wxInt32 tile = knight->tile1().first;
		wxInt32 corner = knight->tile1().second;

		gameData<wxInt32>(shExpelTile) = tile;
		gameData<wxInt32>(shExpelCorner) = corner;

		// Update the UI.
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stExpelsCriminals));

		RULE.Execute(shRulePlaySound, DataObject(SOUND_EXPEL_CRIMINALS));

		// Adjust stats.
		RULE.Execute(shRuleTakeAction, DataObject(current(), shExpelCriminals));

		// Execute the expel.
		RULE.Execute(shRuleExecuteExpelKnight, DataObject());
	}
};

IMPLEMENT_RULE(RuleSelectExpelKnight, CITIES)
