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
#include "SideSelectionObject.h"
#include "Controller.h"
#include "Utility.h"
#include "UtilityGL.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleBuildRoadCommon
// 
// Mixin for handling a click on the road <BuildUI> in the game state of Turn,
// allowing the player to place a new road on the board.
//
// Derived From:
//     <Rule>
//     <IUndoable>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// Mixin To:
//     <RuleBuildRoad>
//
// Rules Called:
//     <RuleUpdateMessageUI>
//
// Logic Called:
//     <LogicCanPlaceRoad>
//
// Selection Rule:
//     <RulePlaceRoadTurn>
//
// Game State:
//     TurnRoadPlacement
//
// Transmits To:
//     eventBuildUI
//     eventSelectionObject
//
class RuleBuildRoadCommon 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
		const HashString& rule = object.read<HashString>();
		const HashString& state = object.read<HashString>(1);

		// Update the UI text.
		RULE.Execute(shRuleUpdateMessageUI, 
			DataObject(stPlaceARoad, stWaitingPlaceARoad));

		// Turn off everything.
		Controller::get().Transmit(shEventBuildUI, DataObject(false, 
			GetGame()));
		Controller::get().Transmit(shEventControlsUI, DataObject(false));

		// Get the set of all available road positions for this player.
		DataObject input(0), output;
		RULE.Decide(shLogicCanPlaceRoad, input, output);

		const TileSideSet &sides = output.read<TileSideSet>();

		wxASSERT(false == sides.empty());

		// Create a SideSelectionObject that holds all of the road placement 
		// possibilities.
		ColorType color = playerGame().color();
		SideSelectionObject *selections = new SideSelectionObject(rule, color);

		float rotation = 0.0f;

		TileSideSet::const_iterator it, itEnd = sides.end();
		for(it = sides.begin(); it != itEnd; ++it)
		{
			Vector coords = tile(it->first)->coords();
			UtilityGL::sideCoords(coords, rotation, it->second);
			selections->add(SideSelectionObject::SideSelectionTuple(
				Utility::encodeSel(it->first, it->second), coords, rotation));
		}

		// This is a new game state.
		gameData<HashString>(shState) = state;

		// Transmit the selections.
		Controller::get().Transmit(shEventSelectionObject, 
			SelectionObjectPtr(selections));

		// For visual help, dim all tiles except those on which the roads can
		// be placed.
		RULE.Execute(shRuleResetObjectFlags, DataObject(IGameObject::Dim));
		RULE.Execute(shRuleResetTileFlags, DataObject(IGameObject::Dim));
		for(it = sides.begin(); it != itEnd; ++it)
		{
			wxInt32 tile1 = it->first;
			wxInt32 tile2 = tile<IntArray>(shSides, tile1)[it->second];

			tile(tile1)->reset();

			if(-1 != tile2)
			{
				tile(tile2)->reset();
			}
		}
	}

	virtual void Unexecute(const DataObject&)
	{
		// Reset the selection.
		Controller::get().Transmit(shEventSelectionObject, 
			SelectionObjectPtr());

		gameData<HashString>(shState) = shTurn;
		RULE.Execute(shRuleRestartTurn, DataObject());
	}

	virtual bool CanUndo() const
	{
		// We need to support undo for backing out of an auto-trade.
		return true;
	}

private:
	typedef std::pair<wxInt32, wxInt32> TileSide;
	typedef std::set<TileSide> TileSideSet;
};

IMPLEMENT_RULE(RuleBuildRoadCommon, STANDARD)
