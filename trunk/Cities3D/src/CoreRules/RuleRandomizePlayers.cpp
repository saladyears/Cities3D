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
#include "RNG.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRandomizePlayers
// 
// Randomizes the order of the players in the <Game> data.
// Iterates through the <PlayerGames>, swapping each with
// a random index.
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
class RuleRandomizePlayers : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		wxInt32 switchVal;

		//make a pass through and randomize
		for(wxInt32 i = 0; i < numPlayers(); i++)
		{
			//get the random value
			switchVal = RAND.pooled(numPlayers());

			//sanity check
			if(switchVal == i) 
			{
				continue;
			}

			std::swap(playerGames()[i], playerGames()[switchVal]);
		}
	}
};

IMPLEMENT_RULE(RuleRandomizePlayers, CORE);
