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
// Class: LogicCardNameScience
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
//     <LogicCardName>
//
class LogicCardNameAlchemist : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stAlchemist);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardNameAlchemist, Alchemist, 
							LogicCardName, CITIES);


// Crane.

class LogicCardNameCrane : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stCrane);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardNameCrane, Crane, 
							LogicCardName, CITIES);


// Engineer.

class LogicCardNameEngineer : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stEngineer);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardNameEngineer, Engineer, 
							LogicCardName, CITIES);

// Inventor.

class LogicCardNameInventor : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stInventor);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardNameInventor, Inventor, 
							LogicCardName, CITIES);

// Irrigation.

class LogicCardNameIrrigation : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stIrrigation);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardNameIrrigation, Irrigation, 
							LogicCardName, CITIES);

// Medicine.

class LogicCardNameMedicine : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stMedicine);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardNameMedicine, Medicine, 
							LogicCardName, CITIES);

// Mining.

class LogicCardNameMining : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stMining);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardNameMining, Mining, 
							LogicCardName, CITIES);

// Printer.

class LogicCardNamePrinter : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stPrinter);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardNamePrinter, Printer, 
							LogicCardName, CITIES);

// Road Building.

class LogicCardNameRoadBuilding : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stRoadBuilding);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardNameRoadBuilding, RoadBuilding, 
							LogicCardName, CITIES);

// Smith.

class LogicCardNameSmith : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stSmith);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardNameSmith, Smith, 
							LogicCardName, CITIES);
