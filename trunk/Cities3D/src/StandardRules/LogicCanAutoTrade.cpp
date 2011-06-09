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
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicCanAutoTrade
// 
// Determines whether the player has enough resources to execute an auto-trade
// for the given resources.
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
class LogicCanAutoTrade : public Logic
{
	void Decide(const DataObject &input, DataObject &output)
	{
		typedef std::map<HashString, wxInt32> ResourceMap;

		// Here is the auto-trade algorithm:
		//
		// 1.  Remove all resources the user already has from the pool of
		//     resources they need to get.
		// 2.  From the resources left to the player, count how many tradeable
		//     resource piles they have.
		// 3.  If they don't have enough tradeable resource piles, they can't
		//     auto-trade.
		// 4.  If the number of tradeable resource piles is equal to the number
		//     of resources left they need to get, they can do an exact 
		//     auto-trade.
		// 5.  If it is more, it means they can auto-trade, but they will have
		//     to manually specify which of the resources to trade to the bank.
		//

		// Create some startup pools of what they need, and what they have.
		ResourceMap resourcesNeeded = input.read<ResourceMap>();
		ResourceMap resourcesInHand;

		const Data::IntHash& playerRes = 
			playerGameData<Data::IntHash>(shResources);
		Data::IntHash::const_iterator it, itEnd = playerRes.end();
		for(it = playerRes.begin(); it != itEnd; ++it)
		{
			resourcesInHand[it->first] = it->second;
		}

		wxInt32 tradesNeeded = 0;

		// Now go through and remove from what's need what the user already
		// has.
		ResourceMap::iterator itThis, itMap, itMapEnd = resourcesNeeded.end();
		itMap = resourcesNeeded.begin();
		while(itMap != itMapEnd)
		{
			itThis = itMap++;

			const HashString& res = itThis->first;

			ResourceMap::iterator itPlayer = resourcesInHand.find(res);
			wxASSERT(resourcesInHand.end() != itPlayer);

			wxInt32 &neededAmount = itThis->second;
			wxInt32 &playerAmount = itPlayer->second;

			// Two possibilities: they have enough or they don't.
			if(playerAmount >= neededAmount)
			{
				playerAmount -= neededAmount;
				resourcesNeeded.erase(itThis);
			}
			else
			{
				neededAmount -= playerAmount;
				resourcesInHand.erase(itPlayer);

				tradesNeeded += neededAmount;
			}
		}

		wxInt32 tradesInHand = 0;

		// What's left in resourcesNeeded is what they need to trade for.
		// Now examine how many resources they have that they can trade.
		itMap = resourcesInHand.begin();
		itMapEnd = resourcesInHand.end();
		while(itMap != itMapEnd)
		{
			itThis = itMap++;

			const HashString& res = itThis->first;
			wxInt32& amount = itThis->second;

			// See if this resource can be bank traded.
			output.reset();
			RULE.Decide(shLogicIsResourceBankTradeable, 
				DataObject(res, amount), output);
			wxASSERT(2 <= output.numItems());

			if(true == output.read<bool>())
			{
				wxInt32 ratio = output.read<wxInt32>(1);

				// Add the number of times they can trade.
				wxInt32 trades = (amount / ratio);
				tradesInHand += trades;

				// Make sure that we store off the exact amount they can trade.
				amount = (trades * ratio);
			}
			// If we can't trade the item, remove it from what counts in our
			// hand.
			else
			{
				resourcesInHand.erase(itThis);
			}
		}
	
		// We may need one final pass through what they have in hand.  If we 
		// have more in hand than we need, but all of the resources in hand are
		// of the same type, then we can still auto-trade because the player 
		// will just turn in all of one type of card.
		if( (tradesInHand > tradesNeeded) &&
			(1 == resourcesInHand.size()))
		{
			itMap = resourcesInHand.begin();

			const HashString& res = itMap->first;
			wxInt32& amount = itMap->second;

			// Get the trade ratio for this resource.
			output.reset();
			RULE.Decide(shLogicIsResourceBankTradeable, 
				DataObject(res, amount), output);
			wxASSERT(2 <= output.numItems());
			wxASSERT(true == output.read<bool>());

			wxInt32 ratio = output.read<wxInt32>(1);

			// Use only what we need.
			amount = (tradesNeeded * ratio);
			tradesInHand = tradesNeeded;
		}

		// If the trades in hand is >= trades needed, they can auto-trade.
		output = DataObject(tradesNeeded, tradesInHand, 
			std::make_pair(resourcesNeeded, resourcesInHand));
	}
};

IMPLEMENT_LOGIC(LogicCanAutoTrade, STANDARD);
