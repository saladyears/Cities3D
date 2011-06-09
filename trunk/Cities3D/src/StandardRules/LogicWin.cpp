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
#include "SideObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicWin
// 
// Determines if a the given player has won in a standard game.  A player wins
// if they have as many or more points than the map specifies, and if they
// are the current player.
//
// Derived From:
//     <Logic>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
class LogicWin : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		wxASSERT(1 <= input.numItems());
		
		bool win = false;

		// Ignore any point adjustments during an undo.
		if(false == RULE.InUndo())
		{
			wxInt32 player = input.read<wxInt32>();
			wxInt32 points = playerGameData<wxInt32>(shPoints, player);
			wxInt32 curPlayer = current();
			wxInt32 winPoints = gameData<wxInt32>(shPoints);

			if( (player == curPlayer) &&
				(points >= winPoints))
			{
				win = true;
			}
		}

		output = DataObject(win);
	}
};

IMPLEMENT_LOGIC(LogicWin, STANDARD);
