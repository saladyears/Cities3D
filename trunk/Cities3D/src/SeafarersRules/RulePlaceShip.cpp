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
#include "Controller.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePlaceShip
// 
// Places a ship on the board at the given tile and side
// for the current player.  Adds a new <ShipObject> to the
// current player's <PlayerGame> data.  Subtracts one from
// player's StockShips.  Plays the ship placement sound.  
// Sends the new <ShipObject> up to the View via the 
// eventGameObjects <Event>.
//
// Derived From:
//     <Rule>,
//     <IUndoable>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <Seafarers>
//
// Rules Called:
//     <RulePlaySound>
//
// Transmits To:
//     eventPlayerUI
//
class RulePlaceShip 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
		// Update the UI.
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stPlacesAShip));

		RULE.Execute(shRulePlaceShipCommon, object);

		//subtract one from the player's stock ships
		playerGameData<wxInt32>(shStockShips) -= 1;

		//play the correct sound
		RULE.Execute(shRulePlaySound, DataObject(SOUND_PLACE_SHIP));
	}

	virtual void Unexecute(const DataObject &)
	{
		// Increment the player's ships.
		playerGameData<wxInt32>(shStockShips) += 1;

		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), -1));

		// Update the UI.
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stUndoesAShip));
	}

	virtual bool CanUndo() const
	{
		// Placing ships should never stop an undo chain.
		return true;
	}
};

IMPLEMENT_RULE(RulePlaceShip, SEAFARERS);
