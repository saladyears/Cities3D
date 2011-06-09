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
// Class: LogicCanAttackKnightTurn
// 
// Determines whether a knight can attack an opponent's knight during the 
// player's turn.
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
class LogicCanAttackKnightTurn : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		// These are the conditions that must be met for a knight to be able to
		// attack another knight:
		//
		// 1)  The knight must be active.
		// 2)  The knight may not have been activated this turn.
		// 3)  The knight must be higher than strength 1.
		// 4)  The player must have a valid place to attack another knight.

		wxInt32 turn = gameData<wxInt32>(shTurn);

		PlayerGame::CornerObjectArray valid;

		PlayerGame::CornerObjectArray knights =
			playerGame().getCornerObjects(shKnights);
		
		PlayerGame::CornerObjectArray::const_iterator it, 
			itEnd = knights.end();

		wxInt32 player = current();

		for(it = knights.begin(); it != itEnd; ++it)
		{
			CornerObjectPtr knight = (*it);

			KnightObject* thisKnight = static_cast<KnightObject*>(
				knight.get());

			wxInt32 strength = thisKnight->strength();

			// Conditions 1, 2 and 3.
			if( (true == thisKnight->active()) &&
				(turn != thisKnight->active_turn()) &&
				(1 < strength))
			{
				// Condition 4.
				DataObject input(knight->tile1(), player, strength), output;
				RULE.Decide(shLogicKnightMovePlaces, input, output);

				const TileCornerSet& corners = output.read<TileCornerSet>();
				
				if(false == corners.empty())
				{
					valid.push_back(knight);
				}
			}
		}

		bool canAttack = false;
		
		if(false == valid.empty())
		{
			canAttack = true;
		}

		output = DataObject(canAttack, 1, valid);
	}

private:
	typedef std::pair<wxInt32, wxInt32> TileCorner;
	typedef std::set<TileCorner> TileCornerSet;
				
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCanAttackKnightTurn, Turn,
							LogicCanAttackKnight, CITIES)
