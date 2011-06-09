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
#include "Rule.h"
#include "RuleSetDatabase.h"
#include "Controller.h"
#include "RNG.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleCurrentOfferHotseat
// 
// In hotseat mode, the other players automatically accept the trade offer
// if they can.
//
// Derived From:
//     <Rule>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
class RuleCurrentOfferHotseat : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		// The right side of the trade offer is what we need to check for, as
		// it is what is requested from the players.
		typedef std::map<HashString, wxInt32> ResourceMap;
		const ResourceMap &left = object.read<ResourceMap>();
		const ResourceMap &right = object.read<ResourceMap>(1);

		wxInt32 curPlayer = current();

		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			if(i != curPlayer)
			{
				ResourceMap thisLeft = left;
				ResourceMap thisRight = right;

				bool accept = true;
				Data::IntHash resources =
					playerGameData<Data::IntHash>(shResources, i);
				size_t anyAmount = 0;

				ResourceMap::const_iterator it, itEnd = thisRight.end();
				for(it = thisRight.begin(); it != itEnd; ++it)
				{
					const HashString& res = it->first;
					wxInt32 amount = it->second;
					
					if(shAnyMatch != res)
					{
						wxInt32 playerAmount = resources[res];
				
						if(amount > playerAmount)
						{
							accept = false;
							break;
						}
						else
						{
							resources[res] -= amount;
						}
					}
					else
					{
						wxASSERT(0 == anyAmount);
						anyAmount = amount;
					}
				}

				// If they had AnyMatch cards, see if we have enough left to
				// randomly fill the order.
				if(0 < anyAmount)
				{
					ResourceMap::iterator itMatch = thisRight.find(shAnyMatch);
					wxASSERT(thisRight.end() != itMatch);

					thisRight.erase(itMatch);

					accept = false;

					// Build an array of all the resources the player has.
					HashStringArray array;
					Data::IntHash::iterator it, itEnd = resources.end();
					for(it = resources.begin(); it != itEnd; ++it)
					{
						const HashString& res = it->first;

						if(thisLeft.end() == thisLeft.find(res))
						{
							wxInt32 amount = it->second;

							for(wxInt32 i = 0; i < amount; ++i)
							{
								array.push_back(res);
							}
						}
					}

					size_t size = array.size();

					// If they have enough, fill it randomly.
					if(anyAmount <= size)
					{
						for(size_t i = 0; i < anyAmount; ++i)
						{
							size_t index = RAND.unpooled(size - i);
							const HashString& res = array[index];
							thisRight[res]++;

							array[index] = array[size - 1 - i];
						}

						// If we have more than 4 types of cards, we're done.
						if(4 >= thisRight.size())
						{
							accept = true;
						}
					}
				}

				// If there are any AnyMatch cards in the left side of the
				// trade, replace them with a random request.
				ResourceMap::iterator itLeft = thisLeft.find(shAnyMatch);
				if(thisLeft.end() != itLeft)
				{
					wxInt32 amount = itLeft->second;
					thisLeft.erase(itLeft);

					HashStringArray array;

					const Data::IntHash& bank = 
						gameData<Data::IntHash>(shBankResources);
					Data::IntHash::const_iterator it, itEnd = bank.end();
					for(it = bank.begin(); it != itEnd; ++it)
					{
						const HashString& res = it->first;

						if(thisRight.end() == thisRight.find(res))
						{
							array.push_back(res);
						}
					}

					// Randomly pick out resources.
					size_t size = array.size();
					for(wxInt32 i = 0; i < amount; ++i)
					{
						size_t index = RAND.unpooled(size);
						const HashString& res = array[index];
						thisLeft[res]++;
					}
				}

				// If they can accept it, send in the offer.
				if(true == accept)
				{
					Controller::get().Transmit(shEventPlayerOffer,
						DataObject(thisRight, thisLeft, i));
				}
				else
				{
					RULE.Execute(shRuleRejectOffer, DataObject(i));
				}
			}
		}
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RuleCurrentOfferHotseat, Hotseat, RuleCurrentOffer, 
						   STANDARD)
