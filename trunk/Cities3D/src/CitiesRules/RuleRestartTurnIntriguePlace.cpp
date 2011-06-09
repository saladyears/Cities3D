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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRestartTurnIntriguePlace
// 
// Restarts the game from a player selecting a location to move their knight
// that was displaced by the intrigue card
//
// Derived From:
//     <Rule>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
// Mixin To:
//     <RuleRestartGame>
//
// Rules Called:
//     <RuleBuildKnight>
//
// Selection Rule:
//     <RuleSelectMoveKnight>
//
// Transmits To:
//     eventBuildUI
//     eventSelectionObject
//
class RuleRestartTurnIntriguePlace : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
        // The current player is now in a blocking action.
        RULE.Execute(shRuleBeginBlockingAction, DataObject(current()));

		// Get all of the available placements.
		wxInt32 thisTile = gameData<wxInt32>(shMoveKnightTile);
		wxInt32 thisSide = gameData<wxInt32>(shMoveKnightCorner);

		// Get all available placmements.
		DataObject input(TileCorner(thisTile, thisSide), current()), output;
		RULE.Decide(shLogicKnightMovePlaces, input, output);

		TileCornerSet corners = output.read<TileCornerSet>();
		wxASSERT(false == corners.empty());

		RULE.Execute(shRuleBuildKnightCommon, DataObject(corners, 
            shRulePlaceIntrigueKnight));

		// Make sure eveything is hidden.
		Controller::get().Transmit(shEventBuildUI, 
			DataObject(false, GetGame()));
		Controller::get().Transmit(shEventControlsUI, DataObject(false));

		// Update the UI text.
		RULE.Execute(shRuleUpdateMessageUI, DataObject(stWithdrawYourKnight, 
            stWaitingWithdrawAKnight));
	}

private:
	typedef std::pair<wxInt32, wxInt32> TileCorner;
	typedef std::set<TileCorner> TileCornerSet;
};

IMPLEMENT_RULE_KEYED_MIXIN(RuleRestartTurnIntriguePlace, 
						   TurnIntriguePlace, RuleRestartGame, CITIES)
