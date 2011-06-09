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
#include "HexObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleBuildShipCommonPlaces
// 
// Common code for ship placements on the board.
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
class RuleBuildShipCommonPlaces 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(2 <= object.numItems());
		
		const TileSideSet &sides = object.read<TileSideSet>();
		wxASSERT(false == sides.empty());

		const HashString& rule = object.read<HashString>(1);

		// Create a SideSelectionObject that holds all of the ship placement 
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

		// Transmit the selections.
		Controller::get().Transmit(shEventSelectionObject, 
			SelectionObjectPtr(selections));

		// For visual help, dim all tiles except those on which the ships can
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

		// In addition, turn on the pirate tile with the angry red outline.
		const HexObjectPtr& pirate = GetGame()->getHexObject(shPirate);
		wxASSERT(pirate);

		wxInt32 pirateTile = pirate->tile();
		if(-1 != pirateTile)
		{
			tile(pirateTile)->reset();
			tile(pirateTile)->set(TileObject::OutlineRed);
		}
	}

	virtual void Unexecute(const DataObject&)
	{
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

IMPLEMENT_RULE(RuleBuildShipCommonPlaces, SEAFARERS)
