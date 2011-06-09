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
// Class: LogicResourceNameStandard
// 
// Provides translated names of standard resources.
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
//     <LogicResourceName>
//
class LogicResourceNameTimber : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stTimber);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicResourceNameTimber, Timber, 
							LogicResourceName, STANDARD);


// Clay.

class LogicResourceNameClay : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stClay);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicResourceNameClay, Clay, 
							LogicResourceName, STANDARD);


// Sheep.

class LogicResourceNameSheep : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stSheep);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicResourceNameSheep, Sheep, 
							LogicResourceName, STANDARD);

// Wheat.

class LogicResourceNameWheat : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stWheat);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicResourceNameWheat, Wheat, 
							LogicResourceName, STANDARD);

// Ore.

class LogicResourceNameOre : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stOre);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicResourceNameOre, Ore, 
							LogicResourceName, STANDARD);


// Desert.

class LogicResourceNameDesert : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stDesert);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicResourceNameDesert, Desert, 
							LogicResourceName, STANDARD);

// Ocean.

class LogicResourceNameOcean : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stOcean);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicResourceNameOcean, Ocean, 
							LogicResourceName, STANDARD);

// Timber Port

class LogicResourceNamePortTimber : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stTwoToOneTimber);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicResourceNamePortTimber, PortTimber, 
							LogicResourceName, STANDARD);

// Clay Port

class LogicResourceNamePortClay : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stTwoToOneClay);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicResourceNamePortClay, PortClay, 
							LogicResourceName, STANDARD);

// Sheep Port

class LogicResourceNamePortSheep : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stTwoToOneSheep);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicResourceNamePortSheep, PortSheep, 
							LogicResourceName, STANDARD);

// Wheat Port

class LogicResourceNamePortWheat : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stTwoToOneWheat);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicResourceNamePortWheat, PortWheat, 
							LogicResourceName, STANDARD);

// Ore Port

class LogicResourceNamePortOre : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stTwoToOneOre);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicResourceNamePortOre, PortOre, 
							LogicResourceName, STANDARD);

// 3-1 Port

class LogicResourceNamePort3 : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stThreeToOne);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicResourceNamePort3, Port3, 
							LogicResourceName, STANDARD);
