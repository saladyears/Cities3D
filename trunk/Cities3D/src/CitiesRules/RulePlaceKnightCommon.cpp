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
#include "KnightObject.h"
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
// Class: RulePlaceKnightCommon
// 
// Places a knight on the board at the given tile and corner for the current 
// player.  
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
class RulePlaceKnightCommon 
	: public Rule
	, public IUndoable
	, public RuleExtensionMixin
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		//first, determine the tile and corner being built on
		wxInt32 tile, corner;
		boost::tie(tile, corner) = Utility::decodeSel(object.read<wxInt32>());

		// See if we also have strength and upgrade information.
		wxInt32 strength = -1;
		wxInt32 upgrade = -1;
		if(3 <= object.numItems())
		{
			strength = object.read<wxInt32>(1);
			upgrade = object.read<wxInt32>(2);
		}

		PlayerGame &playergame(playerGame());
		ColorType color = playergame.color();

		wxInt32 turn = gameData<wxInt32>(shTurn);

		//now create the knight object for this player and add it to their list
		CornerObjectPtr knight(new KnightObject(current(), turn, color, 
			GetGame(), tile, corner));

		// Set strength and upgrade values if needed.
		if(1 < strength)
		{
			KnightObject* thisKnight = 
				static_cast<KnightObject*>(knight.get());
			thisKnight->strength(strength - 1);
			thisKnight->upgrade(true, upgrade);
		}

		playergame.addCornerObject(shKnights, knight);

		//send it to the view
		AggregatorObjectPtr aggregate(new AggregatorObject);
		aggregate->add(knight);

		Controller::get().Transmit(shEventGameObjects, aggregate);
		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), -1));

		// Run an animation if needed.
		RULE.Execute(shRuleAnimatePlacement, DataObject(playergame.player(),
			GameObjectPtr(knight), color));

		// Run any extensions.
		ExecuteExtensions(object);
	}

	virtual void Unexecute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		// Remove the knight from the player's objects.
		wxInt32 tile, corner;
		boost::tie(tile, corner) = Utility::decodeSel(object.read<wxInt32>());
		CornerObject::TileCorner tileCorner(tile, corner);

		const PlayerGame::CornerObjectArray &knights =
			playerGame().getCornerObjects(shKnights);

		CornerObjectPtr knight;

		PlayerGame::CornerObjectArray::const_iterator it, itEnd = knights.end();
		for(it = knights.begin(); it != itEnd; ++it)
		{
			if(true == (*it)->onTileCorner(tileCorner))
			{
				knight = (*it);
				break;
			}
		}

		playerGame().removeCornerObject(shKnights, knight);

		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), -1));
	}

	virtual bool CanUndo() const
	{
		// Placing knights should never stop an undo chain.
		return true;
	}
};

IMPLEMENT_RULE(RulePlaceKnightCommon, CITIES);
