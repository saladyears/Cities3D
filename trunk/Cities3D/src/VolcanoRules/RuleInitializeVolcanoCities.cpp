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

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- RULES            -----------------------------//

//-----------------------------------------------------------------------//
// Class: RuleInitializeVolcanoCities
// 
// Initializes data for the VolcanoCities option.
//
// Derived From:
//     <Rule>
//
// Project:
//     <VolcanoRules>
//
// RuleSet:
//     <Volcano>
//
// Mixin To:
//     <RuleInitializeData>
//
class RuleInitializeVolcanoCities : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			// Initialize space for players to "lose" their metropolises to the
			// volcano.
			playerGameData<IntArray>(shVolcanoMetropolis, i);
			playerGameData<wxInt32>(shMoveMetropolis, i) = FALSE;
		}
	}
};

IMPLEMENT_RULE_MIXIN(RuleInitializeVolcanoCities, RuleInitializeData, 
					 VOLCANO_CITIES)
