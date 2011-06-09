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
#include "LogicBooleanMixin.h"
#include "RuleSetDatabase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicUIAllowColorChange
// 
// Mixin container that determines if a player has sufficient rights to assign
// the given player to a color in the game.  Defaults to true.  Other rules 
// add more restrictions (like Network).
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
class LogicUIAllowColorChange : public LogicBooleanMixin<>
{
	// NOTE: The boolean mixin handles all of the decision making
	// for this class; we just need a unique name for the class
	// so the mixin logic pieces have something to hang off of.
};

IMPLEMENT_LOGIC(LogicUIAllowColorChange, CORE)
