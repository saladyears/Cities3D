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
#include "RuleExtensionMixin.h"
#include "GameEngine.h"
#include "IUndoable.h"
#include "KnightObject.h"
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
// Class: RulePlaceKnight
// 
// Places a knight on the board at the given tile and corner for the current 
// player.  *Undoable*.
//
// Derived From:
//     <Rule>,
//     <IUndoable>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
// Rules Called:
//     <RuleUpdateNetworkUI>
//     <RuleAnimatePlacement>
//
// Transmits To:
//     eventGameObjects
//
class RulePlaceKnight 
	: public Rule
	, public IUndoable
	, public RuleExtensionMixin
{
public:
	virtual void Execute(const DataObject &object)
	{
		gameData<HashString>(shState) = shTurn;

		// This trick allows road lengths to be recalculated properly after
		// an undo.
		RULE.Execute(shRuleLongestRoadCheck, DataObject(false));

		// Clear any dim tiles.
		RULE.Execute(shRuleResetTileFlags, DataObject());
		RULE.Execute(shRuleResetObjectFlags, DataObject());

		// Update the UI.
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stPlacesABasicKnight));

		RULE.Execute(shRulePlaceKnightCommon, object);
		
		Controller::get().Transmit(shEventKnightCounts, GetGame());

		//subtract one from the player's stock knights
		playerGameData<wxInt32>(shStockKnights1) -= 1;

		// Play the knight sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_PLACE_KNIGHT_1));

		// Execute any extensions to knight placement.
		ExecuteExtensions(object);

		// Adjust the resources used.
		DataObject input(0), output;
		RULE.Decide(shLogicCostKnight, input, output);
		RULE.Execute(shRulePurchase, output);

		// Adjust stats.
		RULE.Execute(shRulePurchaseItem, DataObject(current(), shKnight1));

		// Longest Road Check.  A knight placement may break
		// longest road if it cuts longest road in half.
		RULE.Execute(shRuleLongestRoadCheck, DataObject(true));

		// Back to turn actions.
		RULE.Execute(shRuleRestartTurn, DataObject());
	}

	virtual void Unexecute(const DataObject &)
	{
		//add one to the player's stock knights
		playerGameData<wxInt32>(shStockKnights1) += 1;

		// Update the UI.
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stUndoesABasicKnight));
	}

	virtual bool CanUndo() const
	{
		// The Longest Road check will automatically disallow undoes if it
		// alters anything.
		return true;
	}
};

IMPLEMENT_RULE(RulePlaceKnight, CITIES)
