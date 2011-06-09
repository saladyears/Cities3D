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
#include "Utility.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePlaySettlementSound
// 
// Determines the correct sound to play when a new
// settlement is placed on the board.  If the settlement
// is placed on a port, a special port sound is played,
// otherwise, the normal settlement sound is played.
//
// Derived From:
//     <Rule>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// Rules Called:
//     <RulePlaySound>
//
// Logic Called:
//     <LogicTileIsPort>
//
class RulePlaySettlementSound : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		wxInt32 thisTile, corner;
		boost::tie(thisTile, corner) = 
			Utility::decodeSel(object.read<wxInt32>());

		//determine other sides
		wxInt32 tile2, tile3;
		wxInt32 corner2, corner3;
		Utility::otherTiles(GetGame(), thisTile, corner, tile2, corner2, tile3, 
			corner3);

		bool bPort = false;
		wxInt32 portTile = -1, port = -1;

		DataObject input(thisTile), output;
		RULE.Decide(shLogicTileIsPort, input, output);
		
		//check for a port tile
		if(true == output.read<bool>())
		{
			portTile = thisTile; port = corner;
		}

		//check for a port tile
		input = DataObject(tile2); 
		output.reset();
		RULE.Decide(shLogicTileIsPort, input, output);
		
		if(true == output.read<bool>()) 
		{
			portTile = tile2; port = corner2;
		}

		//check for a port tile
		input = DataObject(tile3);
		output.reset();
		RULE.Decide(shLogicTileIsPort, input, output);
		
		if(true == output.read<bool>()) 
		{
			portTile = tile3; port = corner3;
		}

		//see if they have built on a port
		if( (-1  < portTile) && 
			(
			(port == tile<wxInt32>(shPort1, portTile)) ||
			(port == tile<wxInt32>(shPort2, portTile))
			)
			)
		{
			bPort = true;
		}

		DataObject sound(
			bPort ? SOUND_PLACE_SETTLEMENT_ON_PORT : SOUND_PLACE_SETTLEMENT);
		RULE.Execute(shRulePlaySound, sound);
	}
};

IMPLEMENT_RULE(RulePlaySettlementSound, STANDARD);
