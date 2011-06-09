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
#include "Utility.h"
#include "Controller.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRemoveSettlement
// 
// Removes a settlement from the board at the given tile and corner for the
// current player.  Decrement's the player's current points by 1.  Adds one to
// the player's StockSettlements.  *Undoable*.
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
class RuleRemoveSettlement 
: public Rule
, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		wxInt32 player = current();

		// See if a particular player is having it removed.
		if(2 <= object.numItems())
		{
			player = object.read<wxInt32>(1);
		}

		// Get the tile and corner being removed.
		wxInt32 tile, corner;
		boost::tie(tile, corner) = Utility::decodeSel(object.read<wxInt32>());

		// Find the settlement in the player's current list.
		const PlayerGame::CornerObjectArray &settlements = 
			playerGame(player).getCornerObjects(shSettlements);

		CornerObject::TileCorner tileCorner(tile, corner);

		CornerObjectPtr settlement;
		PlayerGame::CornerObjectArray::const_iterator it, 
			itEnd = settlements.end();
		for(it = settlements.begin(); it != itEnd; ++it)
		{
			if(true == (*it)->onTileCorner(tileCorner))
			{
				settlement = (*it);
				break;
			}
		}

		// Remove it from the player's list.
		playerGame(player).removeCornerObject(shSettlements, settlement);

		// Add one to the player's stock settlements.
		playerGameData<wxInt32>(shStockSettlements, player) += 1;

		// Subtract one point from the player's score.
		RULE.Execute(shRuleAdjustPoints, DataObject(player, -1));
	}

	virtual void Unexecute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		wxInt32 player = current();
		if(2 <= object.numItems())
		{
			player = object.read<wxInt32>(1);
		}

		// Replace the settlement on the corner.
		wxInt32 tile, corner;
		boost::tie(tile, corner) = Utility::decodeSel(object.read<wxInt32>());

		wxInt32 turn = gameData<wxInt32>(shTurn);

		CornerObjectPtr settlement(new SettlementObject(player, turn,
			playerGame(player).color(), GetGame(), tile, corner));
		playerGame(player).addCornerObject(shSettlements, settlement);

		// Send it to the view.
		AggregatorObjectPtr aggregate(new AggregatorObject);
		aggregate->add(settlement);

		Controller::get().Transmit(shEventGameObjects, aggregate);

		// Subtract one from the player's stock settlements.
		playerGameData<wxInt32>(shStockSettlements, player) -= 1;

		// Add one point to the player's score.
		RULE.Execute(shRuleAdjustPoints, DataObject(player, 1));
	}

	virtual bool CanUndo() const
	{
		return true;
	}
};

IMPLEMENT_RULE(RuleRemoveSettlement, STANDARD)
