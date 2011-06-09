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
// Class: LogicCardNamePolitics
// 
// Provides translated names of politics progress cards.
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
class LogicCardNameArsonist : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stArsonist);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardNameArsonist, Arsonist, 
							LogicCardName, CITIES);

class LogicCardNameBishop : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stBishop);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardNameBishop, Bishop, 
							LogicCardName, CITIES);


// Constitution.

class LogicCardNameConstitution : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stConstitution);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardNameConstitution, Constitution, 
							LogicCardName, CITIES);


// Deserter.

class LogicCardNameDeserter : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stDeserter);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardNameDeserter, Deserter, 
							LogicCardName, CITIES);

// Diplomat.

class LogicCardNameDiplomat : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stDiplomat);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardNameDiplomat, Diplomat, 
							LogicCardName, CITIES);

// Intrigue.

class LogicCardNameIntrigue : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stIntrigue);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardNameIntrigue, Intrigue, 
							LogicCardName, CITIES);

// Saboteur.

class LogicCardNameSaboteur : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stSaboteur);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardNameSaboteur, Saboteur, 
							LogicCardName, CITIES);

// Spy.

class LogicCardNameSpy : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stSpy);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardNameSpy, Spy, 
							LogicCardName, CITIES);

// Warlord.

class LogicCardNameWarlord : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stWarlord);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardNameWarlord, Warlord, 
							LogicCardName, CITIES);

// Wedding.

class LogicCardNameWedding : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stWedding);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardNameWedding, Wedding, 
							LogicCardName, CITIES);
