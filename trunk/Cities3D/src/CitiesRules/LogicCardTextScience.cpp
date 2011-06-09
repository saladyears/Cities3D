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
// Class: LogicCardTextScience
// 
// Provides translated names of science progress cards.
//
// Derived From:
//     <Logic>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
// Mixin To:
//     <LogicCardText>
//
class LogicCardTextAlchemist : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stBeforeTheDiceAreRolled);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextAlchemist, Alchemist, 
							LogicCardText, CITIES);


// Crane.

class LogicCardTextCrane : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stPurchaseOneCityUpgrade);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextCrane, Crane, 
							LogicCardText, CITIES);


// Engineer.

class LogicCardTextEngineer : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stPlaceACityWallsFree);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextEngineer, Engineer, 
							LogicCardText, CITIES);

// Inventor.

class LogicCardTextInventor : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stSwitchAnyTwoRollChips);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextInventor, Inventor, 
							LogicCardText, CITIES);

// Irrigation.

class LogicCardTextIrrigation : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stReceiveTwoWheatForEach);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextIrrigation, Irrigation, 
							LogicCardText, CITIES);

// Medicine.

class LogicCardTextMedicine : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stBuildACityForTwo);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextMedicine, Medicine, 
							LogicCardText, CITIES);

// Mining.

class LogicCardTextMining : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stReceiveTwoOreForEach);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextMining, Mining, 
							LogicCardText, CITIES);

// Printer.

class LogicCardTextPrinter : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		// Should never get here.
		wxASSERT(false);
		output = DataObject(stOneVictoryPoint);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextPrinter, Printer, 
							LogicCardText, CITIES);

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
							LogicCardText, CITIES);

// Smith.

class LogicCardTextSmith : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stPromoteTwoDifferentKnights);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextSmith, Smith, 
							LogicCardText, CITIES);
