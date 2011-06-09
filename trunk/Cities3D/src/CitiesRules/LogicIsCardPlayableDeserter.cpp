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
#include "Logic.h"
#include "LogicHashMixin.h"
#include "RuleSetDatabase.h"
#include "KnightObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicIsCardPlayableDeserter
// 
// Determines whether the Deserter card is playable right now.
//
// Derived From:
//     <Logic>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
// Mixin To:
//     <LogicIsCardPlayable>
//
class LogicIsCardPlayableDeserter : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		bool canPlay = false;

		// The Deserter is playable if any other player has a knight on the
		// board.
		wxInt32 curPlayer = current();
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			if(i != curPlayer)
			{
				const PlayerGame::CornerObjectArray& knights =
					playerGame(i).getCornerObjects(shKnights);

				if(false == knights.empty())
				{
					canPlay = true;
					break;
				}
			}
		}

		output = DataObject(canPlay);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicIsCardPlayableDeserter, Deserter,
							LogicIsCardPlayableCities, CITIES)
