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
#include "LogicBooleanMixin.h"
#include "RuleSetDatabase.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicCanAcceptTrade
// 
// Determines whether the player has enough resources to meet the trade
// requirements.
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
class LogicCanAcceptTrade : public Logic
{
	void Decide(const DataObject &input, DataObject &output)
	{
		wxASSERT(3 <= input.numItems());

		bool canAccept = true;

		typedef std::map<HashString, wxInt32> ResourceMap;

		// The left side of the trade is always what the player has to give
		// up.
		const ResourceMap& left = input.read<ResourceMap>(0);
		const ResourceMap& right = input.read<ResourceMap>(1);
		wxInt32 index = input.read<wxInt32>(2);

		Data::IntHash& resources = 
			playerGameData<Data::IntHash>(shResources, index);
		ResourceMap::const_iterator it, itEnd = left.end();
		for(it = left.begin(); it != itEnd; ++it)
		{
			const HashString& res = it->first;
			wxInt32 tradeAmount = it->second;

			wxInt32 resAmount = 0;
			
			if(shAnyMatch != res)
			{
				wxASSERT(resources.end() != resources.find(res));
				resAmount = resources[res];
			}			
			
			if(tradeAmount > resAmount)
			{
				canAccept = false;
				break;
			}
		}

		// There can't be any AnyMatch cards on the right either.
		if( (true == canAccept) &&
			(right.end() != right.find(shAnyMatch)))
		{
			canAccept = false;
		}

		output = DataObject(canAccept);
	}
};

IMPLEMENT_LOGIC(LogicCanAcceptTrade, STANDARD);
