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
#include "IRuleEngine.h"
#include "ChipObject.h"
#include "AggregatorObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleSelectSecondInventorChip
// 
// Swaps the given chip with the stored inventor chip.
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
class RuleSelectSecondInventorChip : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
        // The current player has ended a blocking action.
        RULE.Execute(shRuleEndBlockingAction, DataObject(current()));

		wxInt32 secondIndex = object.read<wxInt32>(0);
		wxInt32 firstIndex = gameData<wxInt32>(shInventorChip);
		wxASSERT(secondIndex != firstIndex);

		wxInt32 roll1 = tile<wxInt32>(shRoll, firstIndex);
		wxInt32 roll2 = tile<wxInt32>(shRoll, secondIndex);

		// Swap them.
		tile<wxInt32>(shRoll, firstIndex) = roll2;
		tile<wxInt32>(shRoll, secondIndex) = roll1;

		AggregatorObjectPtr aggregate(new AggregatorObject);
		tile(firstIndex)->createChipObject(true);
		tile(firstIndex)->chip()->randomizeRotation();
		aggregate->add(tile(firstIndex)->chip());
		
		tile(secondIndex)->createChipObject(true);
		tile(secondIndex)->chip()->randomizeRotation();
		aggregate->add(tile(secondIndex)->chip());

		// Send the data up.
		Controller::get().Transmit(shEventGameObjects, aggregate);

		// Notify the UI.
		wxString str = wxString::Format(stSwapsAChipForAChip.c_str(),
			swStringFormat.c_str(), roll1, roll2);
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(str));

		// Play the sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_SWITCH_ROLL_CHIPS));

		Controller::get().Transmit(shEventDirty, 0);
		RULE.Execute(shRulePlayProgressCardEnd, DataObject());
	}
};

IMPLEMENT_RULE(RuleSelectSecondInventorChip, CITIES)
