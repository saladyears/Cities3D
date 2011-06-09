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
// Class: LogicResourceOrderStandard
// 
// Provides resource ordering for standard resources.
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
//     <LogicResourceOrder>
//

// Sheep.

class LogicResourceOrderSheep : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(0);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicResourceOrderSheep, Sheep, 
							LogicResourceOrder, STANDARD);

// Ore.

class LogicResourceOrderOre : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(1);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicResourceOrderOre, Ore, 
							LogicResourceOrder, STANDARD);

// Tiimber.

class LogicResourceOrderTimber : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(2);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicResourceOrderTimber, Timber, 
							LogicResourceOrder, STANDARD);


// Clay.

class LogicResourceOrderClay : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(3);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicResourceOrderClay, Clay, 
							LogicResourceOrder, STANDARD);


// Wheat.

class LogicResourceOrderWheat : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(4);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicResourceOrderWheat, Wheat, 
							LogicResourceOrder, STANDARD);

