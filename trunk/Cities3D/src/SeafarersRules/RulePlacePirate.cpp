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
#include "HexObject.h"
#include "Controller.h"
#include "UtilityGL.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePlacePirate
// 
// Places the pirate on the given tile.  
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
class RulePlacePirate : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		//first, determine the tile and side being built on
		wxInt32 thisTile = object.read<wxInt32>();

		HexObjectPtr pirate = GetGame()->getHexObject(shPirate);
		wxASSERT(pirate);

		pirate->reset();
		pirate->tile(thisTile);

		// Adjust the pirate so it is not on top of port stuff.
		wxInt32 port1 = tile<wxInt32>(shPort1, thisTile);
		Vector coords = tile(thisTile)->coords();

		if(-1 != port1)
		{
			wxInt32 corner = (port1 + 3) % 6;
			UtilityGL::cornerCoords(coords, corner, 0.6f);
		}

		pirate->coords(coords);

		Controller::get().Transmit(shEventDirty, 0);
	}
};

IMPLEMENT_RULE(RulePlacePirate, SEAFARERS);
