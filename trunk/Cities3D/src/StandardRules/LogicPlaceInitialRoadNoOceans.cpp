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
#include "Logic.h"
#include "RuleSetDatabase.h"
#include "GameEngine.h"
#include "IRuleEngine.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
typedef std::pair<wxInt32, wxInt32> TileCorner;

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicPlaceInitialRoadNoOceans
// 
// Prevents an initial road from being built on the 
// intersection of two ocean tiles.
//
// Derived From:
//     <Logic>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// Mixin To:
//     <LogicPlaceInitialRoad>
//
// Logic Called:
//     <LogicTileIsOcean>
//
class LogicPlaceInitialRoadNoOceans : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		wxASSERT(1 <= input.numItems());

		//read the tile and side from the input
		wxInt32 thisTile, side;
		boost::tie(thisTile, side) = input.read<TileCorner>();

		//if both this tile and the opposing tile are oceans, no good
		wxInt32 otherTile = tile<IntArray>(shSides, thisTile)[side];

		//check for oceans
		DataObject ocean1(thisTile), output1;
		RULE.Decide(shLogicTileIsOcean, ocean1, output1);

		DataObject ocean2(otherTile), output2;
		RULE.Decide(shLogicTileIsOcean, ocean2, output2);

		bool valid = ((false == output1.read<bool>()) || 
					  (false == output2.read<bool>()));

		output = DataObject(valid);
	}
};

IMPLEMENT_LOGIC_MIXIN(LogicPlaceInitialRoadNoOceans, LogicPlaceInitialRoad, 
					  STANDARD);
