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
// Class: LogicIsThisPlayer
// 
// Logic mixin to determine if the player at this computer is the current 
// player.  This varies by what mode the game is in.
//
// Derived From:
//     <LogicBooleanMixin>
//
// Project:
//     <CoreRules>
//
// RuleSet:
//     <Core>
//
class LogicIsThisPlayer 
	: public Logic
	, public LogicHashMixin
{
public:
	virtual void Decide(const DataObject& input, DataObject& output)
	{
		// Switch based on mode.
		const HashString& mode = gameData<HashString>(shMode);

		bool valid = DecideHash(mode, input, output);
		
		if(false == valid)
		{
			wxASSERT(false);
		}
	}
};

IMPLEMENT_LOGIC(LogicIsThisPlayer, CORE)
