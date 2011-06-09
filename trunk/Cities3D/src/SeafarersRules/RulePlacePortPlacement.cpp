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
#include "AggregatorObject.h"
#include "PortObject.h"
#include "Controller.h"
#include "ConfigDatabase.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePlacePortPlacement
// 
// Places a port on the given tile with the given orientation.
// Sets the TileType of the tile to PortRandom.  Adjusts the
// rotation of the tile to the given orientation.  Adds a
// <PortObject> to the <TileObject> if that option is turned
// on the <ConfigDatabase>.
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
//     <RuleUpdateNetworkUI>
//     <RuleIncrementTurnsPlayed>
//
// Transmits To:
//     eventGameObjects
//
class RulePlacePortPlacement : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		// The current player has ended a blocking action.
        RULE.Execute(shRuleEndBlockingAction, DataObject(current()));

		wxASSERT(2 <= object.numItems());

		gameData<wxInt32>(shInitialPortTile) = -1;
		gameData<wxInt32>(shInitialPortCount)++;

		wxInt32 thisTile = object.read<wxInt32>();
		wxInt32 sideTile = object.read<wxInt32>(1);

		//determine which side of the starting tile the side tile is on
		wxInt32 side = -1;
		for(wxInt32 i = 0; i < 6; ++i)
		{
			if(sideTile == tile<IntArray>(shSides, thisTile)[i])
			{
				side = i;
				break;
			}
		}

		wxASSERT(-1 != side);

		//set the port values
		tile<wxInt32>(shPort1, thisTile) = side;
		tile<wxInt32>(shPort2, thisTile) = (side + 1) % 6;

		//set the rotation for the port properly
		Vector rotation(0.0, ((side + 4) % 6) * 60., 0.0);
		tile(thisTile)->rotation(rotation);

		//the new tile's type is now PortRandom until set at the end
		tile<HashString>(shTileType, thisTile) = shPortRandom;

		//check the config database to see if we should add a Seafarers port 
		//object
		if(0 != CONFIG.read<wxInt32>(swConfigSectionGeneral, swSeafarersPorts, 2))
		{
			// 0 = never use Seafarers ports
			// 1 = always use Seafarers ports
			// 2 = use Seafarers ports on Seafarers maps

			AggregatorObjectPtr aggregate = 
				AggregatorObjectPtr(new AggregatorObject);

			tile(thisTile)->createPortObject(true);
			aggregate->add(tile(thisTile)->port());

			Controller::get().Transmit(shEventGameObjects, aggregate);
		}

		// Update the UI.
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stPlacesAPortHarbor));

		// Every port placement counts as a turn.
		RULE.Execute(shRuleIncrementTurnsPlayed, DataObject());

		// Let the rule chain take over.
	}
};

IMPLEMENT_RULE(RulePlacePortPlacement, PORT_PLACEMENT)
