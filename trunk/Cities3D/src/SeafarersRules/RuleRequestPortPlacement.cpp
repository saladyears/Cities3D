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
#include "DummySelectionObject.h"
#include "ChipObject.h"
#include "PortObject.h"
#include "Controller.h"
#include "GameEngine.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRequestPortPlacement
// 
// Evaluates tiles for possible port locations.  Examines each tile 
// in the map for Ocean tiles that have at least one neighboring land tile.  
// Updates the <MessageUI> via <RuleUpdateMessageUI> with "place a port 
// harbour".  Dims all non-selectable tiles.  Sets <Game> State to 
// InitialPortPlacement.  Turns on selection in the View via a 
// <DummySelectionObject>.
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
//     <RuleResetTileFlags>
//     <RuleUpdateMessageUI>
//
// Logic Called:
//     <LogicTileIsLand>
//
// Selection Rule:
//     <RuleExaminePortPlacement>
//
// Game State:
//     InitialPortPlacement
//
// Transmits To:
//     eventSelectionObject
//
class RuleRequestPortPlacement : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// The current player is now in a blocking action.
        RULE.Execute(shRuleBeginBlockingAction, DataObject(current()));

		RULE.Execute(shRuleResetTileFlags, DataObject());

		RULE.Execute(shRuleUpdateMessageUI, DataObject(stPlaceAPortHarbor, 
            stWaitingPlaceAPortHarbor));

		//check all available tiles for open oceans that have at least
		//one land tile next to them
		DummySelectionObject *pSelection = 
			new DummySelectionObject(shRuleExaminePortPlacement);
		
		//send the dummy selection object up
		Controller::get().Transmit(shEventSelectionObject, 
			SelectionObjectPtr(pSelection));

		for(wxInt32 i = 0; i < numTiles(); ++i)
		{
			//if it's not an Ocean tile, continue to the next one
			if(shOcean != tile<HashString>(shTileType, i))
			{
				//it's land (or a port), so dim it and its chip so that the 
				//selectable ocean tiles are even more visible
				tile(i)->set(IGameObject::Dim);

				continue;
			}

			bool hasLand = false;

			for(wxInt32 j = 0; j < 6; ++j)
			{
				wxInt32 side = tile<IntArray>(shSides, i)[j];

				//sanity check on border tiles
				if(-1 == side)
				{
					continue;
				}

				DataObject input(side), output;
				RULE.Decide(shLogicTileIsLand, input, output);

				if(true == output.read<bool>())
				{
					hasLand = true;
					break;
				}
			}

			if(true == hasLand)
			{
				tile(i)->set(IGameObject::Select);
			}
			else
			{
				tile(i)->set(IGameObject::Dim);
			}
		}

		//new game state now
		gameData<HashString>(shState) = shInitialPortPlacement;
	}
};

IMPLEMENT_RULE(RuleRequestPortPlacement, PORT_PLACEMENT)
