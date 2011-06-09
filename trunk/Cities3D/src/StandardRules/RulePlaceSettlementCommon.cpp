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
#include "IUndoable.h"
#include "SettlementObject.h"
#include "AggregatorObject.h"
#include "Controller.h"
#include "Utility.h"
#include "AnimationPlacement.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePlaceSettlementCommon
// 
// Handles code common to settlement placements.
//
// Derived From:
//     <Rule>,
//     <IUndoable>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// Rules Called:
//     <RuleAnimatePlacement>
//
// Transmits To:
//     eventGameObjects
//
class RulePlaceSettlementCommon 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		wxInt32 player = current();

		if(2 <= object.numItems())
		{
			player = object.read<wxInt32>(1);
		}

		//first, determine the tile and corner being built on
		wxInt32 tile, corner;
		boost::tie(tile, corner) = Utility::decodeSel(object.read<wxInt32>());

		PlayerGame &playergame(playerGame(player));
		ColorType color = playergame.color();

		wxInt32 turn = gameData<wxInt32>(shTurn);

		//now create the settlement object for this player and add it to their 
		//list
		CornerObjectPtr settlement(CornerObjectPtr(new SettlementObject(
			player, turn, color, GetGame(), tile, corner)));
		playergame.addCornerObject(shSettlements, settlement);

		//subtract one from the player's stock settlements
		playerGameData<wxInt32>(shStockSettlements, player) -= 1;

		//send it to the view
		AggregatorObjectPtr aggregate(new AggregatorObject);
		aggregate->add(settlement);

		Controller::get().Transmit(shEventGameObjects, aggregate);

		// Run an animation if needed.
		RULE.Execute(shRuleAnimatePlacement, DataObject(playergame.player(),
			GameObjectPtr(settlement), color));

		// Adjust stats.
		RULE.Execute(shRulePurchaseItem, DataObject(current(), shSettlement));

		// Add one point to the player's score.
		RULE.Execute(shRuleAdjustPoints, DataObject(player, 1));
	}

	virtual void Unexecute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		wxInt32 player = current();

		if(2 <= object.numItems())
		{
			player = object.read<wxInt32>(1);
		}

		//first, determine the tile and corner being built on
		wxInt32 tile, corner;
		boost::tie(tile, corner) = Utility::decodeSel(object.read<wxInt32>());

		PlayerGame &playergame(playerGame(player));

		wxInt32 turn = gameData<wxInt32>(shTurn);

		//now create the settlement object for this player and remove it from 
		//their list
		CornerObjectPtr settlement(CornerObjectPtr(new SettlementObject(
			player, turn, playergame.color(), GetGame(), tile, corner)));
		playergame.removeCornerObject(shSettlements, settlement);

		//add one to the player's stock settlements
		playerGameData<wxInt32>(shStockSettlements, player) += 1;

		// Subtract one point from the player's score.
		RULE.Execute(shRuleAdjustPoints, DataObject(player, -1));
	}

	virtual bool CanUndo() const
	{
		//default to true for settlement placements, allow
		//container rules to decide
		return true;
	}
};

IMPLEMENT_RULE(RulePlaceSettlementCommon, STANDARD)
