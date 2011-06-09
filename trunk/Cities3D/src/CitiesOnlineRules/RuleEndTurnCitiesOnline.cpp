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
#include "NetworkPlayers.h"
#include "INetworkEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleEndTurnCitiesOnline
// 
// Extension to RuleEndTurn to report data to the site.
//
// Derived From:
//     <Rule>
//
// Project:
//     <CitiesOnlineRules>
//
// RuleSet:
//     <CitiesOnline>
//
// Mixin To:
//     <RuleEndTurn>
//
class RuleEndTurnCitiesOnline : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
        // Disable the turn timer for this player.
        RULE.Execute(shRuleResetTurnTime, DataObject(false));

		// Only report if we're in a Cities Online game and not a spectator.
		if( (TRUE == gameData<wxInt32>(shCitiesOnline)) &&
			(false == NET.Players().IsSpectator()))
		{
			// Get current point total.
			DataObject input(-1), output;
			RULE.Decide(shLogicIsThisPlayer, input, output);
			wxASSERT(2 <= output.numItems());

			wxInt32 actualIndex = output.read<wxInt32>(1);
			wxASSERT(-1 != actualIndex);

			wxInt32 points = playerGameData<wxInt32>(shPoints, actualIndex);

			// Get total resource count.
			input = DataObject(actualIndex);
			output.reset();
			RULE.Decide(shLogicResourceCount, input, output);
			wxASSERT(1 <= output.numItems());

			wxInt32 resources = output.read<wxInt32>();

			// Get total card count.
			input = DataObject(actualIndex);
			output.reset();
			RULE.Decide(shLogicCardCount, input, output);
			wxASSERT(1 <= output.numItems());
			
			wxInt32 cards = output.read<wxInt32>();

			// And, the game id.
			wxInt32 id = gameData<wxInt32>(shID);

			// And the current game turn.
			wxInt32 turn = gameData<wxInt32>(shTurn);

			// Send the command.
			HashString command = HashString::Format(shUpdateCmd.c_str(), id, 
                points, resources, cards, turn);
			Controller::get().Transmit(shEventSynchronize, command);
		}
	}
};

IMPLEMENT_RULE_MIXIN(RuleEndTurnCitiesOnline, RuleEndTurn, CITIESONLINE)
