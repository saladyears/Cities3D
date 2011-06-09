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

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicRequestInitialRoad
// 
// Provides <RuleRequestInitialRoad> with the text "place a road",
// the logic <LogicPlaceInitialRoad>, and the rule <RulePlaceInitialRoad>,
// for the <Game> States of FirstRoad and SecondRoad.
//
// Derived From:
//     <Logic>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// Mixin To:
//     <RuleRequestInitialRoad>
//
class LogicRequestInitialRoad : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stPlaceARoad, stWaitingPlaceARoad, shPlaceInitialRoad);
	}
};

//use this logic twice
namespace
{
	struct sAutoRegister_InitialRoad
	{
		sAutoRegister_InitialRoad(Logic *logic)
		{
			RuleSetDatabase::get().RegisterLogic(STANDARD, LogicPtr(logic), 
				shLogicRequestInitialRoad);
			RuleSetDatabase::get().RegisterMixin(STANDARD, 
				shRuleRequestInitialRoad, shFirstRoad, 
				shLogicRequestInitialRoad);
			RuleSetDatabase::get().RegisterMixin(STANDARD, 
				shRuleRequestInitialRoad, shSecondRoad, 
				shLogicRequestInitialRoad);
		}
	};
	sAutoRegister_InitialRoad object_InitialRoad(new LogicRequestInitialRoad);
}
