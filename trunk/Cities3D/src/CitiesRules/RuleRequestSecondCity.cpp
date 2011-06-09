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
#include "GameEngine.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRequestSecondCity
// 
// Prepares the board for placing a city during the second
// round of initial placements in C&K.  Calls <RuleUpdateMessageUI>
// with "place a city".  Calls <RuleRequestInitialSettlement>
// to prepare possible city selections on the board.  Sets
// the <Game> State to SecondCity.
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
// Rules Called:
//     <RuleUpdateMessageUI>,
//     <RuleRequestInitialSettlement>
//
// Game State:
//     SecondCity
// 
class RuleRequestSecondCity : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		RULE.Execute(shRuleUpdateMessageUI, 
			DataObject(stPlaceACity, stWaitingPlaceACity));

		RULE.Execute(shRuleRequestInitialSettlement,
			DataObject(shRulePlaceSecondCity));
		
		//we're in a new state
		gameData<HashString>(shState) = shSecondCity;
	}
};

IMPLEMENT_RULE(RuleRequestSecondCity, CITIES);
