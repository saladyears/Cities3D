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
// Class: RuleExecuteDiscardCard
// 
// Removes the given progress card from the player and returns it to the deck.
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
class RuleExecuteDiscardCard : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(2 <= object.numItems());

		const HashString& card = object.read<HashString>();
		wxInt32 player = object.read<wxInt32>(1);

        // This player has ended a blocking action.
        RULE.Execute(shRuleEndBlockingAction, DataObject(player));
		
		// This is kind of crappy, but go through each progress card set and
		// find which set the card belongs to.
		HashString type;
        FindType(card, type, player);

		HashString bankType = HashString::Format(shBankStringCards.c_str(), 
            type.c_str());
		HashStringArray& deck = gameData<HashStringArray>(bankType);

		// This hurts.
		deck.insert(deck.begin(), card);

		// Remove this card from the player's hand.
		HashString handType = HashString::Format(shPlayableStringCards.c_str(), 
			type.c_str());
		Data::IntHash& cards = playerGameData<Data::IntHash>(handType, player);

		wxInt32& count = cards[card];
		wxASSERT(0 < count);
		--count;

		DataObject input(type), output;
		RULE.Decide(shLogicUpgradeName, input, output);
		const wxString& upgradeName = output.read<wxString>();
		
		// Now create the text.
		wxString str = wxString::Format(stDiscardsOneCard.c_str(), 
            swStringFormat.c_str(), upgradeName.c_str());
	
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(str, player));

		// TODO: Play the sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_DISCARD_PROGRESS_CARD));

		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), player));
		
		// Any time a player drops to zero of something, we need to repaint.
		Controller::get().Transmit(shEventDirty, 0);

		RULE.Execute(shRuleRestartDiscardProgressCards, DataObject(player));
	}

private:
	void FindType(const HashString& card, HashString& type, wxInt32 player)
	{
        type = shTrade;
        if(false == SearchHash(card, type, player)) 
        {
            type = shPolitics;
            if(false == SearchHash(card, type, player)) 
            {
                type = shScience;
            }
        }
	}

	bool SearchHash(const HashString& card, const HashString& type, 
		wxInt32 player)
	{
		bool ret = false;

		HashString searchType = HashString::Format(shPlayableStringCards.c_str(), 
			type.c_str());
		const Data::IntHash& cards = playerGameData<Data::IntHash>(searchType, 
			player);
		Data::IntHash::const_iterator it, itEnd = cards.end();
		for(it = cards.begin(); it != itEnd; ++it)
		{
			if(it->first == card)
			{
				ret = true;
				break;
			}
		}

		return ret;
	}
};

IMPLEMENT_RULE(RuleExecuteDiscardCard, CITIES)
