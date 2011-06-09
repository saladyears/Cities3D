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
// Class: RuleRequestPortOrientation
// 
// Evaluates the given tile for possible port orientations.  Dims all
// tiles in the map.  Evaluates each neighboring tile of the
// given port placement tile.  If the neighboring tile is a land
// tile, the tile is made selectable and undimmed.  Updates the
// <MessageUI> via <RuleUpdateMessageUI> with "orient the port 
// harbour".  Sets <Game> State to InitialPortPlacement.  
// Turns on selection in the View via a <DummySelectionObject>.
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
//     InitialPortOrientation
//
// Transmits To:
//     eventSelectionObject
//
class RuleRequestPortOrientation : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		// The current player is now in a blocking action.
        RULE.Execute(shRuleBeginBlockingAction, DataObject(current()));

		wxASSERT(1 <= object.numItems());

		RULE.Execute(shRuleUpdateMessageUI, DataObject(stOrientThePortHarbor, 
            stWaitingOrientThePortHarbor));

		//set the dummy selection
		DummySelectionObject *pSelection = new DummySelectionObject(
			shRuleExaminePortPlacement);

		//send the dummy selection object up
		Controller::get().Transmit(shEventSelectionObject, 
			SelectionObjectPtr(pSelection));

		RULE.Execute(shRuleResetTileFlags, DataObject(IGameObject::Dim));

		wxInt32 thisTile = object.read<wxInt32>();

		//turn on tiles around the origin tile
		for(wxInt32 i = 0; i < 6; ++i)
		{
			wxInt32 side = tile<IntArray>(shSides, thisTile)[i];

			if(-1 != side)
			{
				DataObject input(side), output;
				RULE.Decide(shLogicTileIsLand, input, output);

				if(true == output.read<bool>())
				{
					tile(side)->reset();
					tile(side)->set(IGameObject::Select);
				}
			}
		}

		//new game state now
		gameData<HashString>(shState) = shInitialPortOrientation;
	}
};

IMPLEMENT_RULE(RuleRequestPortOrientation, PORT_PLACEMENT)
