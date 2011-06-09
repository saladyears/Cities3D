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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicCanBuildShip
// 
// Mixin container for logic that determines if the
// ship <BuildUI> should be visible, which is true
// if the player can build a ship and has a place to
// put it on the board.  Looks up the <ILogic> in its
// hash based on current the <Game> State.  If no <ILogic>
// exists for that State, the output <DataObject> is
// set to false, and the <BuildUI> is not visible.  Otherwise,
// calls the hashed <ILogic> and returns its output.
//
// Derived From:
//     <Logic>,
//     <LogicHashMixin>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//

class LogicCanBuildShip 
: public Logic
, public LogicHashMixin
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		const HashString& state = gameData<HashString>(shState);

		if(false == DecideHash(state, input, output))
		{
			output = DataObject(false);
		}
	}
};

IMPLEMENT_LOGIC(LogicCanBuildShip, SEAFARERS)
