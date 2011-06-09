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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleUIRemovePlayer
// 
// Removes the given player from the <Game> data.  Finds
// the <PlayerGame> containing the player's <Player> data
// and removes it from the <Game> data.
//
// Derived From:
//     <Rule>
//
// Project:
//     <CoreRules>
//
// RuleSet:
//     <Core>
//
// Transmits To:
//     eventPreGame
//
class RuleUIRemovePlayer : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		//remove the player from the game
		const Player &player = object.read<Player>();
		
		Game::PlayerGameArray::iterator it = playerGames().begin(), 
			itEnd = playerGames().end();

		for(; it != itEnd; ++it)
		{
			if(player == it->player())
			{
				playerGames().erase(it);

				//fire the update mechanism
				Controller::get().Transmit(shEventPreGame, GetGame());
				break;
			}
		}
	}
};

IMPLEMENT_RULE(RuleUIRemovePlayer, CORE);
