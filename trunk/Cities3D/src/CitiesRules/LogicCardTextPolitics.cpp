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
// Class: LogicCardTextPolitics
// 
// Provides translated text of politics progress cards.
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


// Arsonist.

class LogicCardTextArsonist : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stAllPlayersLoseHalfResources);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextArsonist, Arsonist, 
							LogicCardText, CITIES);

// Bishop.

class LogicCardTextBishop : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stMoveTheRobberOrPirate);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextBishop, Bishop, 
							LogicCardText, CITIES);


// Constitution.

class LogicCardTextConstitution : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		// Should never get here.
		wxASSERT(false);
		output = DataObject(stOneVictoryPoint);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextConstitution, Constitution, 
							LogicCardText, CITIES);


// Deserter.

class LogicCardTextDeserter : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stSelectAnOpponentTheyMustRemove);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextDeserter, Deserter, 
							LogicCardText, CITIES);

// Diplomat.

class LogicCardTextDiplomat : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stRemoveOneOpenRoad);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextDiplomat, Diplomat, 
							LogicCardText, CITIES);

// Intrigue.

class LogicCardTextIntrigue : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stDisplaceAnOpponentsKnight);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextIntrigue, Intrigue, 
							LogicCardText, CITIES);

// Saboteur.

class LogicCardTextSaboteur : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stSelectAnOpponentsCity);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextSaboteur, Saboteur, 
							LogicCardText, CITIES);

// Spy.

class LogicCardTextSpy : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stSelectAnOpponentSteal1ProgressCard);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextSpy, Spy, 
							LogicCardText, CITIES);

// Warlord.

class LogicCardTextWarlord : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stActivateAllYourKnights);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextWarlord, Warlord, 
							LogicCardText, CITIES);

// Wedding.

class LogicCardTextWedding : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stAllPlayersGiveTwoResources);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextWedding, Wedding, 
							LogicCardText, CITIES);
