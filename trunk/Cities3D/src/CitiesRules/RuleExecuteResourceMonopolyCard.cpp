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
// Class: RuleExecuteResourceMonopolyCard
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
class RuleExecuteResourceMonopolyCard : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
        // The current player has ended a blocking action.
        RULE.Execute(shRuleEndBlockingAction, DataObject(current()));

		wxASSERT(1 <= object.numItems());

		typedef std::map<HashString, wxInt32> ResourceMap;
		const ResourceMap& resource = object.read<ResourceMap>();
		wxASSERT(1 == resource.size());

		const HashString& res = resource.begin()->first;

		wxString str;
		bool first = true;

		DataObject input(res), output;
		RULE.Decide(shLogicResourceName, input, output);
		const wxString& resName = output.read<wxString>();
		
		// Go through all the players, removing this resource from them.
		wxInt32 stolen = 0;
		wxInt32 curPlayer = current();
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			if(i == curPlayer)
			{
				continue;
			}

			wxInt32 amount = 
				playerGameData<Data::IntHash>(shResources, i)[res];
			
			wxInt32 loss = std::min(amount, 2);

			if(0 < loss)
			{
				const wxString& playerName = playerGame(i).player().Name();

				if(true == first)
				{
					first = false;
				}
				else
				{
					str += swCommaSpace;
				}

				str += wxString::Format(stFrom.c_str(), loss, 
					resName.c_str(), playerName.c_str());

				stolen += loss;

				RULE.Execute(shRuleAdjustResources, DataObject(i, -loss, res));
			}
		}

		str = wxString::Format(stSteals.c_str(), swStringFormat.c_str(), 
            str.c_str());

		// Rub it in if they're stupid.
		if(0 == stolen)
		{
			str = stStealsNothing;
		}

		// Play the sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_PLAY_MONOPOLY_CARD));

		// Give the player their stolen resources.
		RULE.Execute(shRuleAdjustResources, DataObject(curPlayer, stolen, res));
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(str));

		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), -1));
		
		// Any time a player drops to zero of something, we need to repaint.
		Controller::get().Transmit(shEventDirty, 0);

		RULE.Execute(shRulePlayProgressCardEnd, DataObject());
	}
};

IMPLEMENT_RULE(RuleExecuteResourceMonopolyCard, CITIES)
