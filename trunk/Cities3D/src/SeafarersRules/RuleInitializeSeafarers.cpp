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
#include "PirateObject.h"
#include "AggregatorObject.h"
#include "Controller.h"
#include "RuleSetDatabase.h"
#include "Map.h"
#include "GameEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------------------------------------------------------//
// Class: RuleInitializeSeafarers
// 
// Initialize starting data for Seafarers.  Creates all
// <Game> and <PlayerGame> data fields.  Creates the <PirateObject>
// and sends it up to the view via the eventGameObjects
// <Event>.
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
// Mixin To:
//     <RuleInitializeData>
//
// Transmits To:
//     eventGameObjects
//
class RuleInitializeSeafarers : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		//initialize player data needed
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			playerGameData<wxInt32>(shStockShips, i) = MAP.read<wxInt32>(
				shStockShips);
			playerGameData<wxInt32>(shMovedShip, i) = 0;
		}

		//the game should always have a pirate in Seafarers (start it off not 
		//visible)
		HexObjectPtr pirate(new PirateObject(0));
		pirate->set(IGameObject::Hidden);
		GetGame()->setHexObject(shPirate, pirate);

		//send it off to the view
		AggregatorObjectPtr aggregate(new AggregatorObject);
		aggregate->add(pirate);
		Controller::get().Transmit(shEventGameObjects, aggregate);
	}
};

IMPLEMENT_RULE_MIXIN(RuleInitializeSeafarers, RuleInitializeData, SEAFARERS)
