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
#include "RuleSetDatabase.h"
#include "KnightObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicCanExpelKnight
// 
// Determines whether a knight can expel the robber or pirate.
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
// Logic Called:
//
class LogicCanExpelKnight : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		// These are the conditions that must be met for a knight to be able to
		// expel the robber or pirate:
		//
		// 1)  The knight must be active.
		// 2)  The knight may not have been activated this turn.
		// 3)  The knight must be on a tile bordering the robber or pirate.

		wxInt32 turn = gameData<wxInt32>(shTurn);

		PlayerGame::CornerObjectArray valid;

		PlayerGame::CornerObjectArray knights =
			playerGame().getCornerObjects(shKnights);
		
		PlayerGame::CornerObjectArray::const_iterator it, 
			itEnd = knights.end();

		for(it = knights.begin(); it != itEnd; ++it)
		{
			CornerObjectPtr knight = (*it);

			KnightObject* thisKnight = static_cast<KnightObject*>(
				knight.get());

			// Conditions 1 and 2.
			if( (true == thisKnight->active()) &&
				(turn != thisKnight->active_turn()))
			{
				// Condition 3.
				DataObject input(knight->tile1()), output;
				RULE.Decide(shLogicCanExpelCorner, input, output);
	
				if(true == output.read<bool>())
				{
					valid.push_back(knight);
				}
			}
		}

		bool canExpel = false;
		
		if(false == valid.empty())
		{
			canExpel = true;
		}

		output = DataObject(canExpel, 1, valid);
	}
};

IMPLEMENT_LOGIC(LogicCanExpelKnight, CITIES)
