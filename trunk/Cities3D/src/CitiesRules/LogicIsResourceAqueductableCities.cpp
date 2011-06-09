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
// Class: LogicIsResourceAqueductableCities
// 
// Disallows commodities from showing up in the Aqueduct dialog.
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
//     <LogicIsResourceAqueductable>
//
class LogicIsResourceAqueductablePaper : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(false);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicIsResourceAqueductablePaper, Paper, 
							LogicIsResourceAqueductable, CITIES);

// Cloth.

class LogicIsResourceAqueductableCloth : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(false);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicIsResourceAqueductableCloth, Cloth, 
							LogicIsResourceAqueductable, CITIES);

// Coin.

class LogicIsResourceAqueductableCoin : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(false);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicIsResourceAqueductableCoin, Coin, 
							LogicIsResourceAqueductable, CITIES);
