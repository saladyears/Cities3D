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
#include "Map.h"
#include "GameEngine.h"
#include "UtilityMap.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleGenerateHidden
// 
// Generates hidden tiles.  Hidden tiles not revealed on the 
// map until a player builds a road or ship orthogonal to them.
// When uncovered, the player receives one free resource of the
// tile type, if possible.
//
// Derived From:
//     <Rule>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <Hidden>
//
// Mixin To:
//     <RuleGenerateBoard>
//
class RuleGenerateHidden : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		//fill the hidden array
		wxInt32 size;
		size = UtilityMap::prepareDataArray(MAP.read<Data::IntHash>(
			shHiddenResources), 
			gameData<HashStringArray>(shHiddenArray));
		wxASSERT(MAP.read<wxInt32>(shHiddenTiles) == size);
		
		//fill the hidden chip array
		size = UtilityMap::prepareDataArray(MAP.read<Data::IntHash>(
			shHiddenChipArray), gameData<HashStringArray>(
			shHiddenChipArray));
		wxASSERT(MAP.read<wxInt32>(shHiddenChips) == size);
	}
};

IMPLEMENT_RULE_MIXIN(RuleGenerateHidden, RuleGenerateBoard, HIDDEN_TILES);
