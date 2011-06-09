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
// Class: LogicCanMoveKnightTurn
// 
// Determines whether a knight can be moved during the player's turn.
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
class LogicCanMoveKnightTurn : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		// These are the conditions that must be met for a knight to be able to
		// be moved:
		//
		// 1)  The knight must be active.
		// 2)  The knight may not have been activated this turn.
		// 3)  The player must have a valid place to move the knight.

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

			// Conditions 1 and 2.
			if( (true == thisKnight->active()) &&
				(turn != thisKnight->active_turn()))
			{
				DataObject input(knight->tile1(), player), output;
				RULE.Decide(shLogicKnightMovePlaces, input, output);

				const TileCornerSet& corners = output.read<TileCornerSet>();
				
				if(false == corners.empty())
				{
					valid.push_back(knight);
				}
			}
		}

		bool canMove = false;
		
		if(false == valid.empty())
		{
			canMove = true;
		}

		output = DataObject(canMove, 1, valid);
	}

private:
	typedef std::pair<wxInt32, wxInt32> TileCorner;
	typedef std::set<TileCorner> TileCornerSet;
				
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCanMoveKnightTurn, Turn,
							LogicCanMoveKnight, CITIES)
