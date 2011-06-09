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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleInitializeHidden
// 
// Initializes hidden tile data.
//
// Derived From:
//     <Rule>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <Hidden>
//
// Mixin To:
//     <RuleInitializeData>
//
class RuleInitializeHidden : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		//initialize game data needed
		gameData<wxInt32>(shHiddenIndex) = 0;	
		gameData<wxInt32>(shHiddenChipIndex) = 0;

		gameData<HashStringArray>(shHiddenArray).resize(0);
		gameData<HashStringArray>(shHiddenChipArray).resize(0);
	}
};

IMPLEMENT_RULE_MIXIN(RuleInitializeHidden, RuleInitializeData, HIDDEN_TILES)
