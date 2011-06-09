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
#include "RuleSetDatabase.h"
#include "GameEngine.h"
#include "ShipObject.h"
#include "AggregatorObject.h"
#include "Controller.h"
#include "Utility.h"
#include "RuleExtensionMixin.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePlaceShipCommon
// 
// Places a ship on the board at the given tile and side for the current 
// player.  Adds a new <ShipObject> to the current player's <PlayerGame> data.
// Sends the new <ShipObject> up to the View via the eventGameObjects <Event>.
//
// Derived From:
//     <Rule>
//     <IUndoable>
//     <RuleExtensionMixin>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <Seafarers>
//
// Rules Called:
//     <RuleUpdateNetworkUI>
//     <RuleAnimatePlacement>
//
// Transmits To:
//     eventGameObjects,
//     eventPlayerUI
//
class RulePlaceShipCommon 
	: public Rule
	, public IUndoable
	, public RuleExtensionMixin
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		//first, determine the tile and side being built on
		wxInt32 tile, side;
		boost::tie(tile, side) = Utility::decodeSel(object.read<wxInt32>());

		PlayerGame &playergame(playerGame());
		ColorType color = playergame.color();

		wxInt32 turn = gameData<wxInt32>(shTurn);

		//now create the ship object for this player and add it to their list
		SideObjectPtr ship(new ShipObject(current(), turn, color, GetGame(), 
			tile, side));
		playergame.addSideObject(shShips, ship);

		//send it to the view
		AggregatorObjectPtr aggregate(new AggregatorObject);
		aggregate->add(ship);

		Controller::get().Transmit(shEventGameObjects, aggregate);
		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), -1));

		// Run an animation if needed.
		RULE.Execute(shRuleAnimatePlacement, DataObject(playergame.player(),
			GameObjectPtr(ship), color));

		// Adjust stats.
		RULE.Execute(shRulePurchaseItem, DataObject(current(), shShip));

		// Run any extensions.
		ExecuteExtensions(object);
	}

	virtual void Unexecute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		// Remove the ship from the player's objects.
		wxInt32 tile, side;
		boost::tie(tile, side) = Utility::decodeSel(object.read<wxInt32>());
		SideObject::TileSide tileSide(tile, side);

		const PlayerGame::SideObjectArray &ships =
			playerGame().getSideObjects(shShips);

		SideObjectPtr ship;

		PlayerGame::SideObjectArray::const_iterator it, itEnd = ships.end();
		for(it = ships.begin(); it != itEnd; ++it)
		{
			if(true == (*it)->onTileSide(tileSide))
			{
				ship = (*it);
				break;
			}
		}

		playerGame().removeSideObject(shShips, ship);

		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), -1));
	}

	virtual bool CanUndo() const
	{
		// Placing ships should never stop an undo chain.
		return true;
	}
};

IMPLEMENT_RULE(RulePlaceShipCommon, SEAFARERS);
