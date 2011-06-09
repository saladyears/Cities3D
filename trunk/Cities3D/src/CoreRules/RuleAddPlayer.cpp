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
// Class: RuleAddPlayer
// 
// Adds the given player to the <Game>.  If the player does not come in with an
// assigned color, assigns the next available color in the <Game> for that 
// player.  Creates a new <PlayerGame> for the player and adds it to the 
// <Game> data.
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
class RuleAddPlayer : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(2 <= object.numItems());

		Player player = object.read<Player>();
		ColorType color = object.read<ColorType>(1);

		//get the next available color if it is requested
		if(CR_SIZE == color)
		{
			//find an available color
			IntArray colors;
			colors.resize(CR_SIZE);

			for(wxInt32 i = 0; i < numPlayers(); ++i)
			{
				colors[playerGame(i).color()] = TRUE;
			}

			IntArray::const_iterator it;
			it = std::find(colors.begin(), colors.end(), FALSE);

			color = static_cast<ColorType>(it - colors.begin());
		}

		//add the player to the game
		PlayerGame game(player);
		game.color(color);
		playerGames().push_back(game);
	}
};

IMPLEMENT_RULE(RuleAddPlayer, CORE);
