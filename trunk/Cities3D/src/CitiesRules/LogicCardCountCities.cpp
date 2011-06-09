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
#include "LogicHashMixin.h"
#include "RuleSetDatabase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicCardCountCities
// 
// Logic that counts the number of cards the given player has in hand.
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
class LogicCardCountCities : public Logic
{
public:
	virtual void Decide(const DataObject& input, DataObject& output)
	{
		wxASSERT(1 <= input.numItems());
		wxInt32 index = input.read<wxInt32>();

		// Sum up their trade, politics, and science cards.
		wxInt32 sum = 0;
		sum += CardCount(index, shPlayableTradeCards);
		sum += CardCount(index, shPlayablePoliticsCards);
		sum += CardCount(index, shPlayableScienceCards);
		
		output = DataObject(sum);
	}

private:
	wxInt32 CardCount(wxInt32 index, const HashString& type)
	{
		const Data::IntHash &hash = 
			playerGameData<Data::IntHash>(type, index);

		wxInt32 sum = 0;
		Data::IntHash::const_iterator it, itEnd(hash.end());
		for(it = hash.begin(); it != itEnd; ++it)
		{
			sum += it->second;
		}

		return sum;
	}
};

IMPLEMENT_LOGIC_REPLACE(LogicCardCountCities, CITIES, LogicCardCount)
