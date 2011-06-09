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
#include "HTML.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkRulePlayerReady
// 
// Marks a player in the game as ready to start playing.
//
// Derived From:
//     <Rule>
//
// Project:
//     <NetworkRules>
//
// RuleSet:
//     <Network>
//
// Rules Called:
//     <RuleGenerateSystemMessage>
//
// Transmits To:
//     eventPreGame
//
class NetworkRulePlayerReady : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		const Player &player = object.read<Player>();
		
		// Find the player in the current game and mark them as ready.
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			const Player &indexPlayer = playerGame(i).player();

			if(player == indexPlayer)
			{
				playerGameData<wxInt32>(shReady, i) = TRUE;
				Controller::get().Transmit(shEventPreGame, GetGame());

				wxString str = wxString::Format(stIsReady.c_str(),
					player.Name().c_str());
				HTML::Color(str, playerGame(i).color());

				RULE.Execute(shRuleGenerateSystemMessage, DataObject(str));
				break;
			}
		}
	}
};

IMPLEMENT_RULE(NetworkRulePlayerReady, NETWORK)
