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
#include "DummySelectionObject.h"
#include "HexObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleStartRobberPirate
// 
// Starts up the logic to play the robber or pirate.
//
// Derived From:
//     <Rule>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <Seafarers>
//
class RuleStartRobberPirate : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		RULE.Execute(shRuleUpdateMessageUI, DataObject(stPlaceTheRobberOrPirate, 
            stWaitingPlaceTheRobberOrPirate));

		//set the dummy selection
		DummySelectionObject *pSelection = new DummySelectionObject(
			shRuleExecuteRobberPiratePlacement);

		//send the dummy selection object up
		Controller::get().Transmit(shEventSelectionObject, 
			SelectionObjectPtr(pSelection));

		// Make sure eveything is hidden.
		Controller::get().Transmit(shEventBuildUI, 
			DataObject(false, GetGame()));
		Controller::get().Transmit(shEventControlsUI, DataObject(false));

		RULE.Execute(shRuleResetTileFlags, DataObject(IGameObject::Dim));

		// They can't place the robber on the current robber tile.
		const HexObjectPtr& robber = GetGame()->getHexObject(shRobber);
		wxInt32 robberTile = robber->tile();

		// They also can't place the pirate on the current pirate tile.
		const HexObjectPtr& pirate = GetGame()->getHexObject(shPirate);
		wxInt32 pirateTile = pirate->tile();

		// Turn on all land and ocean tiles except the robber and pirate tiles.
		wxInt32 tiles = numTiles();
		for(wxInt32 i = 0; i < tiles; ++i)
		{
			if( (i != robberTile) &&
				(i != pirateTile))
			{
				DataObject input(i), output;
				RULE.Decide(shLogicTileIsLand, input, output);
				bool isLand = output.read<bool>();

				output.reset();
				RULE.Decide(shLogicTileIsOcean, input, output);
				bool isOcean = output.read<bool>();

				if( (true == isLand) ||
					(true == isOcean))
				{
					tile(i)->reset();
					tile(i)->set(IGameObject::Select);
				}
			}
		}
	}
};

// Replace the rule in the Seafarers ruleset so that we can handle the pirate.
IMPLEMENT_RULE_REPLACE(RuleStartRobberPirate, RuleStartRobber, SEAFARERS)
