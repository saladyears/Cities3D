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
// Class: LogicCanTradeOthersStandard
// 
// Determines whether the player can trade with other players on their turn.
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
// Mixin To:
//     <LogicCanTradeOthers>
//
class LogicCanTradeOthersStandard : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		// In order to trade, a player must have at least one resource, and
		// at least on other player must have at least one resource.

		wxInt32 curPlayer = current();
		bool currentTrade = false;
		bool otherTrade = false;

		if(shTurn == gameData<HashString>(shState))
		{
			for(wxInt32 i = 0; i < numPlayers(); ++i)
			{
				wxInt32 total = 0;

				const Data::IntHash& resources = 
					playerGameData<Data::IntHash>(shResources, i);
				Data::IntHash::const_iterator it, itEnd = resources.end();
				for(it = resources.begin(); it != itEnd; ++it)
				{
					total += it->second;
				}

				if(0 < total)
				{
					if(i == curPlayer)
					{
						currentTrade = true;
					}
					else
					{
						otherTrade = true;
					}
				}
			}
		}

		output = DataObject(currentTrade && otherTrade);
	}
};

IMPLEMENT_LOGIC_MIXIN(LogicCanTradeOthersStandard, LogicCanTradeOthers, 
					  STANDARD)
