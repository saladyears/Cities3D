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
// Class: RuleExecuteMasterMerchant
// 
// Takes the given resource from all players and gives it to the current
// player, to a maximum of 2 cards.
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
class RuleExecuteMasterMerchant : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
        // The current player has ended a blocking action.
        RULE.Execute(shRuleEndBlockingAction, DataObject(current()));

		wxASSERT(1 <= object.numItems());

		wxInt32 currentIndex = current();
		wxInt32 victimIndex = gameData<wxInt32>(shMasterMerchantVictim);

		typedef std::map<HashString, wxInt32> ResourceMap;
		const ResourceMap& resource = object.read<ResourceMap>();
		wxASSERT(1 <= resource.size());

		wxInt32 total = 0;
		wxString str;
		bool first = true;

		ResourceMap::const_iterator it, itEnd = resource.end();
		for(it = resource.begin(); it != itEnd; ++it)
		{
			const HashString& res = it->first;
			wxInt32 amount = it->second;

			DataObject input(res), output;
			RULE.Decide(shLogicResourceName, input, output);
			const wxString& resName = output.read<wxString>();

			if(true == first)
			{
				first = false;
			}
			else
			{
				str += swCommaSpace;
			}

			str += wxString::Format(swIntStringFormat.c_str(), amount, resName.c_str());

			// Take from victim, add to player.
			RULE.Execute(shRuleAdjustResources, DataObject(victimIndex, -amount, res));
			RULE.Execute(shRuleAdjustResources, DataObject(currentIndex, amount, res));

			total += amount;
		}

		str = wxString::Format(stStealsFromYou.c_str(), swStringFormat.c_str(), 
			str.c_str());

		// Let the victim know.
		DataObject input(victimIndex), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);
		if(false == output.read<bool>())
		{
			wxString name = playerGame(victimIndex).player().Name();

			if(1 == total)
			{
				str = wxString::Format(stStealsOneCardFrom.c_str(), 
                    swStringFormat.c_str(), name.c_str());
			}
			else
			{
				wxASSERT(2 == total);
				str = wxString::Format(stStealsTwoCardsFrom.c_str(), 
                    swStringFormat.c_str(), name.c_str());
			}
		}

		RULE.Execute(shRuleUpdateNetworkUI, DataObject(str));

		// Play the sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_STEAL_CARD));

		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), currentIndex));
		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), victimIndex));
		
		// Any time a player drops to zero of something, we need to repaint.
		Controller::get().Transmit(shEventDirty, 0);

		RULE.Execute(shRulePlayProgressCardEnd, DataObject());
	}
};

IMPLEMENT_RULE(RuleExecuteMasterMerchant, CITIES)
