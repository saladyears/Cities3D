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
// Class: RuleProgressCardCommon
// 
// Handles one player getting a progress card.
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
class RuleProgressCardCommon : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(2 <= object.numItems());

		wxInt32 thisPlayer = object.read<wxInt32>();
		const HashString& type = object.read<HashString>(1);
		
		HashString bankType = HashString::Format(shBankStringCards.c_str(), 
            type.c_str());
		HashString playerType = HashString::Format(shPlayableStringCards.c_str(), 
			type.c_str());
		HashString playedType = HashString::Format(shPlayedStringCards.c_str(), 
			type.c_str());
		
		HashStringArray& cards = gameData<HashStringArray>(bankType);
		
		if(false == cards.empty())
		{
			HashString thisCard = cards.back();
			cards.pop_back();

			// Update the players.
			RULE.Execute(shRuleUpdateNetworkUI, DataObject(stReceivesAProgressCard, 
				thisPlayer));

			// Tell just this player what the card is.
			DataObject input(-1), output;
			
			RULE.Decide(shLogicCardName, DataObject(thisCard), output);
			wxString name = output.read<wxString>();
				
			output.reset();
			RULE.Decide(shLogicIsThisPlayer, DataObject(thisPlayer), output);
			if(true == output.read<bool>())
			{
				wxString str = wxString::Format(
					stYouReceiveACard.c_str(), name.c_str());
				RULE.AsynchExecute(shRuleUpdateNetworkUI, DataObject(str,
					thisPlayer));

				// Play a sound.
				RULE.Execute(shRulePlaySound, DataObject(SOUND_RECEIVE_PROGRESS_CARD));
			}

			// If a victory point is drawn, it is played immediately.
			if( (shPrinter == thisCard) ||
				(shConstitution == thisCard))
			{
				RULE.Execute(shRulePlayCard, DataObject(thisCard, thisPlayer, name));
			}
			else
			{
				playerGameData<Data::IntHash>(playerType, 
					thisPlayer)[thisCard]++;
			}

			// Update UI.
			Controller::get().Transmit(shEventPlayerUI, 
				DataObject(GetGame(), thisPlayer));
		}
	}
};

IMPLEMENT_RULE(RuleProgressCardCommon, CITIES)
