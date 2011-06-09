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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleExecuteWedding
// 
// Removes the given cards from the given player and gives them to the
// Wedding player.
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
class RuleExecuteWedding : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(2 <= object.numItems());

		typedef std::map<HashString, wxInt32> ResourceMap;
		const ResourceMap& resources = object.read<ResourceMap>();

		wxString str;
		wxInt32 total = 0;
		bool first = true;

		wxInt32 losePlayer = object.read<wxInt32>(1);

        // This player has ended a blocking action.
        RULE.Execute(shRuleEndBlockingAction, DataObject(losePlayer));

		// Execute the transactions.
		ResourceMap::const_iterator it, itEnd = resources.end();
		for(it = resources.begin(); it != itEnd; ++it)
		{
			const HashString& res = it->first;
			wxInt32 amount = it->second;

			// Adjust counts.

			RULE.Execute(shRuleAdjustResources, DataObject(losePlayer, -amount, res));
			RULE.Execute(shRuleAdjustResources, DataObject(current(), amount, res));

			DataObject input(res), output;
			RULE.Decide(shLogicResourceName, input, output);
			const wxString& name = output.read<wxString>();

			if(true == first)
			{
				first = false;
			}
			else
			{
				str += swCommaSpace;
			}

			str += wxString::Format(swIntStringFormat.c_str(), amount, name.c_str());

			total += amount;
		}
		
		// See if this is the player getting cards.
		DataObject input(current()), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);
		if(true == output.read<bool>())
		{
			str = wxString::Format(stGivesToYouAsAWeddingGift.c_str(),
				swStringFormat.c_str(), str.c_str());
		}
		else
		{
			if(1 == total)
			{
				str = wxString::Format(
					stGivesOneCardAsAWeddingGift.c_str(),
					swStringFormat.c_str(), playerGame().player().Name().c_str());
			}
			else
			{
				str = wxString::Format(
					stGivesTwoCardsAsAWeddingGift.c_str(),
					swStringFormat.c_str(), playerGame().player().Name().c_str());
			}
		}

		// Play the sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_GIVE_WEDDING_GIFT));

		// This player no longer needs to lose cards.
		playerGameData<wxInt32>(shWedding, losePlayer) = FALSE;

		RULE.Execute(shRuleUpdateNetworkUI, DataObject(str, losePlayer));

		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), losePlayer));
		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), current()));

		// If a player goes to zero cards, the screen needs to be redrawn.
		Controller::get().Transmit(shEventDirty, 0);
		
		// See if more cards need to be lost.
		RULE.Execute(shRuleRestartTurnWedding, DataObject(false));
	}
};

IMPLEMENT_RULE(RuleExecuteWedding, CITIES)
