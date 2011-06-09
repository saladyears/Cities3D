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
#include "RuleHashMixin.h"
#include "GameEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleAutoSave
// 
// Auto saves a game to the incomplete game database, if allowed by the current
// mode.
//
// Derived From:
//     <Rule>
//     <RuleHashMixin>
//
// Project:
//     <CoreRules>
//
// RuleSet:
//     <Core>
//
class RuleAutoSave 
	: public Rule
	, public RuleHashMixin
{
public:
	virtual void Execute(const DataObject &object)
	{
		const HashString &mode = gameData<HashString>(shMode);

		ExecuteHash(mode, object);
	}
};

IMPLEMENT_RULE(RuleAutoSave, CORE);
