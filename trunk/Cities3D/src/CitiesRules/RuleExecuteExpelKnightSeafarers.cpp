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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleExecuteExpelKnightSeafarers
// 
// Expels the robber or pirate, depending on what the selected knight is near.
//
// Derived From:
//     <Rule>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
class RuleExecuteExpelKnightSeafarers : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		wxInt32 tile = gameData<wxInt32>(shExpelTile);
		wxInt32 corner = gameData<wxInt32>(shExpelCorner);

		// First, determine what this knight borders.
		typedef std::pair<wxInt32, wxInt32> TileCorner;
		TileCorner tileCorner = TileCorner(tile, corner);

		DataObject input(tileCorner), output;
		RULE.Decide(shLogicCanExpelCornerRobber, input, output);
		bool robber = output.read<bool>();

		output.reset();
		RULE.Decide(shLogicCanExpelCornerPirate, input, output);
		bool pirate = output.read<bool>();

		// If they can do both, just send them down the normal robber handling
		// path which does both.
		if( (true == robber) &&
			(true == pirate))
		{
			RULE.Execute(shRuleStartRobber, DataObject());
		}
		// Otherwise, we have to turn on just one or the other.
		else if(true == robber)
		{
			RULE.Execute(shRuleStartRobberCommon, DataObject());
		}
		else if(true == pirate)
		{
			RULE.Execute(shRuleStartPirateCommon, DataObject());
		}
		else
		{
			wxASSERT(false);
		}
	}
};

IMPLEMENT_RULE_REPLACE(RuleExecuteExpelKnightSeafarers, RuleExecuteExpelKnight,
					   CITIES_SEAFARERS)
