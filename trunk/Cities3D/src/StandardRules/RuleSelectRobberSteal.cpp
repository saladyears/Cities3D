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
#include "RobberObject.h"
#include "CornerObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleSelectRobberSteal
// 
// Handles a click on the city or settlement the player wants to steal from.
//
// Derived From:
//     <Rule>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <DevCards>
//
class RuleSelectRobberSteal : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
        // The current player has ended a blocking action.
        RULE.Execute(shRuleEndBlockingAction, DataObject(current()));

		// Clear everything.
		RULE.Execute(shRuleResetTileFlags, DataObject());
		RULE.Execute(shRuleResetObjectFlags, DataObject());

		// Grab the current robber tile.
		HexObjectPtr robber = GetGame()->getHexObject(shRobber);
		wxASSERT(robber);
		wxInt32 robberTile = robber->tile();

		wxInt32 id = object.read<wxInt32>();

		// See if they clicked the robber, meaning they don't want
		// to steal, so deal with that and move on.
		if(RobberObject::ROBBER_ID == id)
		{
			RULE.Execute(shRulePlaySound, DataObject(SOUND_NO_STEAL));

			RULE.Execute(shRuleUpdateNetworkUI, DataObject(stStealsNothing));
		}
		else
		{

			// We've got the id of the object coming in, so match it up against
			// the right player to steal from.
			DataObject output;
			RULE.Decide(shLogicRobberStealPlayers, DataObject(robberTile), 
				output);
			const PlayerGame::CornerObjectArray& objects =
				output.read<PlayerGame::CornerObjectArray>(1);
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

IMPLEMENT_RULE(RuleSelectRobberSteal, STANDARD)
