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
#include "UtilityMap.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleGenerateCities
//
// Creates decks of progress cards.
//
// Derived From:
//     <Rule>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
// Mixin To:
//     <RuleGenerateBoard>
//
class RuleGenerateCities : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// Create all three decks of cards and store them in the game data.
		// We do this here so that any options may alter the deck in 
		// RuleInitializeData prior to RuleGenerateBoard being called.  That
		// way, this rule only creates decks of what cards are available.
		// After creating the decks, the original card counts are erased as
		// they are no longer needed.

		// Trade cards.
		HashStringArray array;
		UtilityMap::prepareDataArray(GetGame()->read<Data::IntHash>(
			shBankTradeCards), array);
		eraseGameData<Data::IntHash>(shBankTradeCards);
		gameData<HashStringArray>(shBankTradeCards) = array;

		// Politics cards.
		UtilityMap::prepareDataArray(GetGame()->read<Data::IntHash>(
			shBankPoliticsCards), array);
		eraseGameData<Data::IntHash>(shBankPoliticsCards);
		gameData<HashStringArray>(shBankPoliticsCards) = array;

		// Science cards.
		UtilityMap::prepareDataArray(GetGame()->read<Data::IntHash>(
			shBankScienceCards), array);
		eraseGameData<Data::IntHash>(shBankScienceCards);
		gameData<HashStringArray>(shBankScienceCards) = array;
	}
};

IMPLEMENT_RULE_MIXIN(RuleGenerateCities, RuleGenerateBoard, CITIES);

