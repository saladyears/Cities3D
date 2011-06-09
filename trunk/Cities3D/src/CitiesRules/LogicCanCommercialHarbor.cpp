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
// Class: LogicCanCommercialHarbor
// 
// Determines whether the player has a commercial harbor that can be used.
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
class LogicCanCommercialHarbor : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		// There are two conditions that must be met for a player to be able
		// to use a commercial harbor:
		// 
		// 1) They must have at least one resource card in hand.
		// 2) There must be at least one opponent they can still steal 
		//    a commodity card from.
		//
		// None of these conditions can be affected by other rulesets.

		wxInt32 curPlayer = current();

		bool hasResources = false;
		bool hasCommodities = false;

		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			const Data::IntHash& resources =
				playerGameData<Data::IntHash>(shResources, i);
			
			wxInt32 resourceCount = 0;
			wxInt32 commodityCount = 0;

			Data::IntHash::const_iterator it, itEnd = resources.end();
			for(it = resources.begin(); it != itEnd; ++it)
			{
				const HashString& res = it->first;
				wxInt32 amount = it->second;

				// Total commodities.
				if( (shPaper == res) ||
					(shCloth == res) ||
					(shCoin == res))
				{
					commodityCount += amount;
				}
				else
				{
					resourceCount += amount;
				}
			}

			if(i == curPlayer)
			{
				if(0 < resourceCount)
				{
					hasResources = true;
				}
			}
			else if( 
				(TRUE == playerGameData<wxInt32>(shCommercialHarbor, i)) &&
				(0 < commodityCount)
				)
			{
				hasCommodities = true;
			}
		}

		output = DataObject(hasResources && hasCommodities, 0);
	}
};

IMPLEMENT_LOGIC(LogicCanCommercialHarbor, CITIES)
