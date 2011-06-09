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
// Class: RuleChangeColor
// 
// Changes the color for the given player to the given color.  If a second 
// player already has the given color, the second player is assigned the first
// player's old color.
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
class RuleChangeColor : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(2 <= object.numItems());

		//read the data out of the object
		Player player = object.read<Player>();
		ColorType color = object.read<ColorType>(1);

		//search through the player array, locating the player who's color is 
		//changing, and any player who already has that color
		Game::PlayerGameArray::iterator it, it1 = playerGames().end(), 
			it2 = playerGames().end();

		for(it = playerGames().begin(); it != playerGames().end(); ++it)
		{
			if(it->player() == player)
			{
				it1 = it;
			}
			if(it->color() == color)
			{
				it2 = it;
			}
		}

		//must have at least found the player
		wxASSERT(playerGames().end() != it1);

		//see if this is a swap
		if(playerGames().end() != it2)
		{
			it2->color(it1->color());
		}

		//otherwise, it's just a reassign
		it1->color(color);
	}
};

IMPLEMENT_RULE(RuleChangeColor, CORE);
