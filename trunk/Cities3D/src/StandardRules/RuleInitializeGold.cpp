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
#include "Controller.h"
#include "RuleSetDatabase.h"
#include "GameEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------------------------------------------------------//
// Class: RuleInitializeGold
// 
// Initialize starting data for Gold.
//
// Derived From:
//     <Rule>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Gold>
//
// Mixin To:
//     <RuleInitializeData>
//
// Transmits To:
//     eventGameObjects
//
class RuleInitializeGold : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		//initialize player data needed
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			// This spot will always be temporary, but initialize it all the
			// same.
			playerGameData<Data::IntHash>(shResources, i)[shGold] = 0;
		}

		// Set gold to indicate that there is an infinite supply of it.
		gameData<Data::IntHash>(shBankResources)[shGold] = INT_MIN;
	}
};

IMPLEMENT_RULE_MIXIN(RuleInitializeGold, RuleInitializeData, GOLD)
