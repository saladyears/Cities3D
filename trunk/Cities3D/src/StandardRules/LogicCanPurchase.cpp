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
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicCanPurchase
// 
// Determines if a player has the resources available to make a purchase.
//
// Derived From:
//     <Logic>
//
// Project:
//     <StandarRules>
//
// RuleSet:
//     <Standard>
//
class LogicCanPurchase : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		typedef std::map<HashString, wxInt32> ResourceMap;
		const ResourceMap &resources = input.read<ResourceMap>();

		bool canPurchase = true;

		ResourceMap::const_iterator it, itEnd = resources.end();
		for(it = resources.begin(); it != itEnd; ++it)
		{
			// See if the user has enough of the resource to cover the cost.
			const wxInt32 resource = playerGameData<Data::IntHash>(
				shResources)[it->first];
			if(0 > resource - it->second)
			{
				canPurchase = false;
				break;
			}
		}

		wxInt32 trades = 0;

		// If they don't have enough straight-up, then see if they can do it
		// with auto-trading.
		if(false == canPurchase)
		{
			RULE.Decide(shLogicCanAutoTrade, input, output);
			wxInt32 tradesNeeded = output.read<wxInt32>();
			wxInt32 tradesInHand = output.read<wxInt32>(1);

			canPurchase = (tradesInHand >= tradesNeeded);

			trades = tradesNeeded;
		}

		output = DataObject(canPurchase, trades);
	}
};

IMPLEMENT_LOGIC(LogicCanPurchase, STANDARD)
