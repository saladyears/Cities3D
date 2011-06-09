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
#include "NetworkPlayers.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicIsThisPlayerHotseat
// 
// Mixin logic for <LogicIsThisPlayer> that determines the player at this
// computer is the current player in a hotseat game.
//
// Derived From:
//     <Logic>
//
// Project:
//     <CoreRules>
//
// RuleSet:
//     <Core>
//
// Mixin To:
//     <LogicIsThisPlayer>
//
class LogicIsThisPlayerHotseat : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(true, current());
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicIsThisPlayerHotseat, Hotseat, 
							LogicIsThisPlayer, HOTSEAT)
