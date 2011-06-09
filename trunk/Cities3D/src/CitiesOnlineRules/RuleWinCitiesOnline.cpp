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
#include "NetworkPlayers.h"
#include "INetworkEngine.h"
#include "Controller.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleWinCitiesOnline
// 
// Sends the command up to the site when the game has been won.
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
//     <RulePostWin>
//
class RuleWinCitiesOnline : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// Report if we're not a spectator.
		if( (TRUE == gameData<wxInt32>(shCitiesOnline)) && 
            (false == NET.Players().IsSpectator()))
		{
			// Grab the id of the winning player to send to the site.
			const Player &currentPlayer = playerGame().player();

			// Also get how many points we finished with.
			DataObject input(-1), output;
			RULE.Decide(shLogicIsThisPlayer, input, output);
			wxASSERT(2 <= output.numItems());
			
			wxInt32 actualIndex = output.read<wxInt32>(1);
			wxASSERT(-1 != actualIndex);

			wxInt32 points = playerGameData<wxInt32>(shPoints, actualIndex);

			// And, the game id.
			wxInt32 id = gameData<wxInt32>(shID);

			HashString command = HashString::Format(shWinCmd.c_str(), id, 
                currentPlayer.Id(), points);
			Controller::get().Transmit(shEventSynchronize, command);
		}
	}
};

IMPLEMENT_RULE_MIXIN(RuleWinCitiesOnline, RulePostWin, CITIESONLINE)
