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
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicResourceCount
// 
// Mixin container for logic that determines whether the total number of
// resources for the given player.
//
// Derived From:
//     <Logic>
//     <BaseMixin>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
class LogicResourceCount 
	: public Logic
	, public BaseMixin
{
	void Decide(const DataObject &input, DataObject &output)
	{
		wxASSERT(1 <= input.numItems());

		// Count all resources in the standard ruleset, and allow any mixins
		// to add to the count.
		wxInt32 playerIndex = input.read<wxInt32>();
		wxInt32 count = 0;

		count += playerGameData<Data::IntHash>(shResources, playerIndex)[shTimber];
		count += playerGameData<Data::IntHash>(shResources, playerIndex)[shClay];
		count += playerGameData<Data::IntHash>(shResources, playerIndex)[shSheep];
		count += playerGameData<Data::IntHash>(shResources, playerIndex)[shWheat];
		count += playerGameData<Data::IntHash>(shResources, playerIndex)[shOre];
		
		IRuleEngine::StringHash::const_iterator it, itEnd(hash_end());
		for(it = hash_begin(); it != itEnd; ++it)
		{
			// Allow plugins to add to the total.
			output.reset();
			RULE.Decide(it->second, input, output);
			wxASSERT(1 <= output.numItems());
		}

		output = DataObject(count);
	}
};

IMPLEMENT_LOGIC(LogicResourceCount, STANDARD);
