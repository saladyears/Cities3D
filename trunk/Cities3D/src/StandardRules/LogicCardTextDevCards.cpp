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
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicCardTextDevCards
// 
// Provides translated descriptions of development cards.
//
// Derived From:
//     <Logic>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <DevCards>
//
// Mixin To:
//     <LogicCardText>
//
class LogicCardTextVictoryPoint : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stReceiveOneVictoryPoint);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextVictoryPoint, VictoryPoint, 
							LogicCardText, DEVCARDS);


// Road Building.

class LogicCardTextRoadBuilding : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stInThisTurnBuildTwoRoads);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextRoadBuilding, RoadBuilding, 
							LogicCardText, DEVCARDS);


// YearOfPlenty.

class LogicCardTextYearOfPlenty : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stInThisTurnTakeTwoCards);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextYearOfPlenty, YearOfPlenty, 
							LogicCardText, DEVCARDS);

// Monopoly.

class LogicCardTextMonopoly : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stChooseOneResourceType);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextMonopoly, Monopoly, 
							LogicCardText, DEVCARDS);

// Soldier.

class LogicCardTextSoldier : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stMoveTheRobberAndSteal);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextSoldier, Soldier, 
							LogicCardText, DEVCARDS);
