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
// Class: LogicResourceOrder
// 
// A mixin container for looking up the image key for a resource, as well as
// its place in the layout order.
//
// Derived From:
//     <Logic>
//     <LogicHashMixin>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
class LogicResourceOrder 
	: public Logic
	, public LogicHashMixin
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		// Some resources (i.e. gold), are special and return -1 as their
		// order.  This flag tells the order logic whether to include them
		// in the order list.  If they are included, they are appended at the
		// end.
		bool special = input.read<bool>();
		wxInt32 specialPlace = 100;

		// Create a map of the resources in the game in the order they are
		// supposed to be displayed in.
		typedef std::map<wxInt32, HashString> OrderMap;
		OrderMap map;

		// Iterate through all the resources in the bank, using the hash logic
		// to get the correct ordering.
		const Data::IntHash& resources = 
			gameData<Data::IntHash>(shBankResources);
		Data::IntHash::const_iterator it, itEnd = resources.end();
		for(it = resources.begin(); it != itEnd; ++it)
		{
			output.reset();
			const HashString& res = it->first;

			// Look up the resource place.
			bool found = DecideHash(res, input, output);
			if(false == found)
			{
				wxASSERT(false);
			}

			wxInt32 place = output.read<wxInt32>();

			if(-1 != place)
			{
				map[place] = res;
			}
			else if(true == special)
			{
				map[specialPlace] = res;
				specialPlace++;
			}
		}

		output = DataObject(map);
	}
};

IMPLEMENT_LOGIC(LogicResourceOrder, STANDARD)
