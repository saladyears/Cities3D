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
#include "IUndoable.h"
#include "Utility.h"
#include "RuleSetDatabase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePlaceInitialTradeRoutes
// 
// 
//
// Derived From:
//     <Rule>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <TradeRoutes>
//
// Mixin To:
//     <RulePlaceInitialExtensions>
//
class RulePlaceInitialTradeRoutes 
: public Rule
, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
		//if this player does not have a home island, set it
		wxInt32 &island = playerGameData<wxInt32>(shHomeIsland);

		if(0 == island)
		{
			wxInt32 thisTile, corner;
			boost::tie(thisTile, corner) = 
				Utility::decodeSel(object.read<wxInt32>());

			island = tile<wxInt32>(shIsland, thisTile);
		}
	}

	virtual void Unexecute(const DataObject &)
	{
		//if the player has only one corner object, then reset their
		//home island back to zero
		if(1 == playerGame().countCornerObjects())
		{
			playerGameData<wxInt32>(shHomeIsland) = 0;
		}
	}

	virtual bool CanUndo() const
	{
		//always undoable
		return true;
	}
};

IMPLEMENT_RULE_MIXIN(RulePlaceInitialTradeRoutes, RulePlaceInitialExtensions, 
					 TRADE_ROUTES)
