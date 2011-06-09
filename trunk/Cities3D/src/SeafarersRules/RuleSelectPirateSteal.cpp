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
#include "PirateObject.h"
#include "SideObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleSelectPirateSteal
// 
// Handles a click on the ship the player wants to steal from.
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
class RuleSelectPirateSteal : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		// The current player has ended a blocking action.
        RULE.Execute(shRuleEndBlockingAction, DataObject(current()));

		// Clear everything.
		RULE.Execute(shRuleResetTileFlags, DataObject());
		RULE.Execute(shRuleResetObjectFlags, DataObject());

		// Grab the current pirate tile.
		HexObjectPtr pirate = GetGame()->getHexObject(shPirate);
		wxASSERT(pirate);
		wxInt32 pirateTile = pirate->tile();

		wxInt32 id = object.read<wxInt32>();

		// See if they clicked the pirate, meaning they don't want
		// to steal, so deal with that and move on.
		if(PirateObject::PIRATE_ID == id)
		{
			RULE.Execute(shRuleUpdateNetworkUI, DataObject(stStealsNothing));
		}
		else
		{
			// We've got the id of the object coming in, so match it up against
			// the right player to steal from.
			DataObject output;
			RULE.Decide(shLogicPirateStealPlayers, DataObject(pirateTile), 
				output);
			const PlayerGame::SideObjectArray& objects =
				output.read<PlayerGame::SideObjectArray>(1);
			const IntArray& owners = output.read<IntArray>(2);

			wxInt32 index = -1;
			for(size_t i = 0; i < objects.size(); ++i)
			{
				if(objects[i]->id() == id)
				{
					index = i;
					break;
				}
			}
			wxASSERT(-1 != index);

			// Grab the owner of the object.
			wxInt32 owner = owners[index];

			// Steal from them.
			RULE.Execute(shRuleExecuteRobberSteal, DataObject(owner));
		}

		// We're done, move on.
		RULE.Execute(shRuleRestartOldState, DataObject());
	}
};

IMPLEMENT_RULE(RuleSelectPirateSteal, SEAFARERS)
