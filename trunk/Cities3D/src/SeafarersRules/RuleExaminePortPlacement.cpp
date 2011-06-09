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
#include "GameEngine.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleExaminePortPlacement
// 
// Examines the user-selected tile for port placement or orientation.
// If the user has previously selected a port tile and has now
// selected the land tile it harbours, calls <RulePlacePortPlacement> with
// the initial port tile and now selected land tile.  If the incoming
// tile *is* the initial port tile, examines the neighboring tiles.  If
// one and only one neighboring tile is a land tile, calls 
// <RulePlacePortPlacement> with the port tile and the one neighboring land 
// tile.  If more than one neighboring tile is a land tile, calls 
// <RuleRequestPortOrientation> with the initial port tile.
//
// Derived From:
//     <Rule>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <PortPlacement>
//
// Rules Called:
//     <RulePlacePortPlacement>,
//     <RuleRequestPortOrientation>
//
// Logic Called:
//     <LogicTileIsLand>
//
class RuleExaminePortPlacement : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		wxInt32 tileID = object.read<wxInt32>();

		wxInt32 currentTile = gameData<wxInt32>(shInitialPortTile);

		//if this is the first tile in the selection, see if it can
		//just be set or if an orientation needs to happen
		if(-1 == currentTile)
		{
			//now, examine the tile in question to see how many land tiles
			//it has around it; if there's just one, we can immediately set the
			//port, otherwise, the user has to select which land tile it goes 
			//to
			wxInt32 landCount = 0;
			wxInt32 landTile = -1;
			for(wxInt32 i = 0; i < 6; ++i)
			{
				wxInt32 side = tile<IntArray>(shSides, tileID)[i];

				if(-1 != side)
				{
					DataObject input(side), output;
					RULE.Decide(shLogicTileIsLand, input, output);

					if(true == output.read<bool>())
					{
						landTile = side;
						++landCount;
					}
				}
			}

			wxASSERT(0 < landCount);

			//if there's just one, call the rule that sets it
			if(1 == landCount)
			{
				gameData<wxInt32>(shInitialPortTile) = -1;

				RULE.Execute(shRulePlacePortPlacement, 
					DataObject(tileID, landTile));
			}
			//otherwise, go on to the orientation request
			else
			{
				gameData<wxInt32>(shInitialPortTile) = tileID;

				RULE.Execute(shRuleRequestPortOrientation, object);
			}
		}
		else
		{
			RULE.Execute(shRulePlacePortPlacement, 
				DataObject(currentTile, tileID));
		}
	}
};

IMPLEMENT_RULE(RuleExaminePortPlacement, PORT_PLACEMENT)
