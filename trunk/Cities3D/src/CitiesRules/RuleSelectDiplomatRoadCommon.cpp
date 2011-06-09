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
#include "IRuleEngine.h"
#include "SideObject.h"
#include "Controller.h"
#include "define/defineGL.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleSelectDiplomatRoadCommon
// 
// Removes the selected road.
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
//     <RulePlayCard>
//
class RuleSelectDiplomatRoadCommon : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
        // The current player has ended a blocking action.
        RULE.Execute(shRuleEndBlockingAction, DataObject(current()));

		wxInt32 id = object.read<wxInt32>();
		const HashString& name = object.read<HashString>(1);
		const wxString& text = object.read<wxString>(2);

		HashString type = HashString::Format(shString.c_str(), name.c_str());
		HashString stock = HashString::Format(shStockString.c_str(), 
            name.c_str());

		wxInt32 player = -1;
		
		// We don't know which player the object belongs to, so look through
		// all objects on the board.
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			const PlayerGame::SideObjectArray& objects = 
				playerGame(i).getSideObjects(type);

			PlayerGame::SideObjectArray::const_iterator it, 
				itEnd = objects.end();
			for(it = objects.begin(); it != itEnd; ++it)
			{
				if((*it)->id() == id)
				{
					break;
				}
			}

			// When we find it, remove it from the player in question and
			// put it back in their stock.
			if(objects.end() != it)
			{
				playerGameData<wxInt32>(stock, i) += 1;
				playerGame(i).removeSideObject(type, (*it));
				Controller::get().Transmit(shEventPlayerUI, 
					DataObject(GetGame(), i));
				player = i;

				// This may have altered Longest Road.
				RULE.Execute(shRuleLongestRoadCheck, DataObject(true));

				break;
			}
		}
		
		if(-1 != player)
		{
			// Play the sound.
			RULE.Execute(shRulePlaySound, DataObject(SOUND_SELECT_DIPLOMAT_ROAD));

			wxString output = wxString::Format(text, swStringFormat.c_str(), 
				playerGame(player).player().Name().c_str());
			RULE.Execute(shRuleUpdateNetworkUI, DataObject(output));

			Controller::get().Transmit(shEventDirty, 0);

			// If the player removed their own object, they may immediately 
			// place it.  Otherwise, they continue their turn.
			if(player == current())
			{
				gameData<HashString>(shDiplomatType) = name;
				gameData<HashString>(shState) = shTurnDiplomatPlace;
				RULE.Execute(shRuleRestartTurnDiplomatPlace, DataObject());
			}
			else
			{
				RULE.Execute(shRulePlayProgressCardEnd, DataObject());
			}
		}
	}
};

IMPLEMENT_RULE(RuleSelectDiplomatRoadCommon, CITIES)
