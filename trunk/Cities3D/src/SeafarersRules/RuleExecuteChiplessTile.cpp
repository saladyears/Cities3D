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
#include "ChipObject.h"
#include "AggregatorObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleExecuteChiplessTile
// 
// Moves a chip from the selected tile to the chipless tile.
//
// Derived From:
//     <Rule>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <ChiplessTiles>
//
class RuleExecuteChiplessTile : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		// The current player has ended a blocking action.
        RULE.Execute(shRuleEndBlockingAction, DataObject(current()));

		gameData<HashString>(shState) = shTurn;

		wxInt32 id = object.read<wxInt32>();

		// Take the chip away from the tile and give it to the chipless tile.
		wxInt32 roll = tile<wxInt32>(shRoll, id);
		wxASSERT(-1 != roll);
		tile(id)->createChipObject(false);

		wxInt32 chiplessTile = gameData<wxInt32>(shChiplessTile);
		tile<wxInt32>(shRoll, chiplessTile) = roll;
		tile<wxInt32>(shChipless, chiplessTile) = FALSE;
		tile<wxInt32>(shRoll, id) = -1;

		eraseGameData<wxInt32>(shChiplessTile);

		// Create the chip for the new tile.
		AggregatorObjectPtr aggregate(new AggregatorObject);
		tile(chiplessTile)->createChipObject(true);
		tile(chiplessTile)->chip()->randomizeRotation();
		aggregate->add(tile(chiplessTile)->chip());

		//notify the display engine
		Controller::get().Transmit(shEventGameObjects, aggregate);

		wxString str = wxString::Format(stTakesFromTheMainland.c_str(),
            swStringFormat.c_str(), roll);
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(str));

		// Restart the turn again.
		RULE.Execute(shRuleRestartTurn, DataObject());
	}
};

IMPLEMENT_RULE(RuleExecuteChiplessTile, CHIPLESS_TILES)
