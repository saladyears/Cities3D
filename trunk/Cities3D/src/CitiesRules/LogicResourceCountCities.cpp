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
#include "CornerObject.h"
#include "HexObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicResourceCountCities
// 
// Counts the total number of commodities in hand for the given player.
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
//     <LogicResourceCount>
//
class LogicResourceCountCities : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		wxASSERT(1 <= input.numItems());

		wxInt32 playerIndex = input.read<wxInt32>();
		wxInt32 count = 0;

		count += playerGameData<Data::IntHash>(shResources, playerIndex)[shPaper];
		count += playerGameData<Data::IntHash>(shResources, playerIndex)[shCloth];
		count += playerGameData<Data::IntHash>(shResources, playerIndex)[shCoin];
		
		output = DataObject(count);
	}
};

IMPLEMENT_LOGIC_MIXIN(LogicResourceCountCities, LogicResourceCount, CITIES)
