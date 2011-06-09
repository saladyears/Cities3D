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
#include "DummySelectionObject.h"
#include "ChipObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRestartTurnInventorChip
// 
// Restarts the game from a player selecting the second tile to switch.
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
class RuleRestartTurnInventorChip : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
        // The current player is now in a blocking action.
        RULE.Execute(shRuleBeginBlockingAction, DataObject(current()));

		RULE.Execute(shRuleUpdateMessageUI, DataObject(stSelectTheSecondRollChip, 
            stWaitingSelectTheSecondRollChip));

		Controller::get().Transmit(shEventControlsUI, DataObject(false));
		Controller::get().Transmit(shEventBuildUI, 
				DataObject(false, GetGame()));
	
		// Transmit the selections.
		Controller::get().Transmit(shEventSelectionObject, 
			SelectionObjectPtr(
			new DummySelectionObject(shRuleSelectSecondInventorChip)));

		// For visual help, dim all tiles except those on which the walls
		// can be placed.
		RULE.Execute(shRuleResetTileFlags, DataObject(IGameObject::Dim));
		RULE.Execute(shRuleResetObjectFlags, DataObject(IGameObject::Dim));

		wxInt32 first = gameData<wxInt32>(shInventorChip);

		// Turn on all tiles that have a roll chip that is not 2, 12, 6, or 8.
		wxInt32 tiles = numTiles();
		for(wxInt32 i = 0; i < tiles; ++i)
		{
			if(i != first)
			{
				const ChipObjectPtr& chip = tile(i)->chip();
				wxInt32 roll = 2;

				if(chip)
				{
					roll = chip->roll();
				}

				if( (2 != roll) &&
					(6 != roll) &&
					(8 != roll) &&
					(12 != roll))
				{
					tile(i)->reset();
					tile(i)->set(IGameObject::Select);
				}
			}
			else
			{
				tile(i)->reset();
				tile(i)->set(IGameObject::Outline);
				tile(i)->set(IGameObject::Dim);
			}
		}
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RuleRestartTurnInventorChip, TurnInventorChip, 
						   RuleRestartGame, CITIES)
