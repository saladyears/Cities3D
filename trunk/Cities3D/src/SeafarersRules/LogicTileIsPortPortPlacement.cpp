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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicTileIsPortPortPlacement
// 
// Mixes into <LogicTileIsPort>.  Adds PortRandom
// to port tile types.
//
// Derived From:
//     <Logic>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <PortPlacement>
//
// Mixin To:
//     <LogicTileIsPort>
//
class LogicTileIsPortPortPlacement : public Logic
{
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		wxASSERT(1 <= input.numItems());

		//look up the tile type
		wxInt32 thisTile = input.read<wxInt32>();
		wxASSERT(0 <= thisTile);

		HashString type = tile<HashString>(shTileType, thisTile);

		bool found = (type == shPortRandom);
		output = DataObject(found);
	}
};

IMPLEMENT_LOGIC_MIXIN(LogicTileIsPortPortPlacement, LogicTileIsPort, 
					  PORT_PLACEMENT);
