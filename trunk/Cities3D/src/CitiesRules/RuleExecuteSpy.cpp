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
// Class: RuleExecuteSpy
// 
// Takes the given progress card from the victim and gives it to the current
// player.
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
class RuleExecuteSpy : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
        // The current player has ended a blocking action.
        RULE.Execute(shRuleEndBlockingAction, DataObject(current()));

		wxASSERT(1 <= object.numItems());

		wxInt32 currentIndex = current();
		wxInt32 victimIndex = gameData<wxInt32>(shSpyVictim);

		const HashString& card = object.read<HashString>();
		
		// This is kind of crappy, but go through each progress card set and
		// find which set the card belongs to.
		HashString type = FindType(card);

		// Adjust the card counts for each player.
		Data::IntHash& victimCards = 
			playerGameData<Data::IntHash>(type, victimIndex);
		Data::IntHash& currentCards =
			playerGameData<Data::IntHash>(type);

		victimCards[card] -= 1;
		wxASSERT(0 <= victimCards[card]);
		currentCards[card] += 1;

		DataObject input(card), output;
		RULE.Decide(shLogicCardName, input, output);
		wxString cardName = output.read<wxString>();
		
		// Now create the text.
		wxString str;

		input = DataObject(victimIndex);
		output.reset();
		RULE.Decide(shLogicIsThisPlayer, input, output);
		if(true == output.read<bool>())
		{
			str = wxString::Format(stStealsOneFromYou.c_str(),
				swStringFormat.c_str(), cardName.c_str());
		}
		else
		{
			wxString name = 
				playerGame(victimIndex).player().Name();
			str = wxString::Format(stStealsOneProgressCardFrom.c_str(),
				swStringFormat.c_str(), name.c_str());
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

private:
	HashString FindType(const HashString& card)
	{
		HashString type = shPlayableTradeCards;
		if(false == SearchHash(card, type))
		{
			type = shPlayablePoliticsCards;
			if(false == SearchHash(card, type))
			{
				type = shPlayableScienceCards;
				wxASSERT(true == SearchHash(card, type));
			}
		}

		return type;
	}

	bool SearchHash(const HashString& card, const HashString& type)
	{
		bool ret = false;

		const Data::IntHash& cards = playerGameData<Data::IntHash>(type);
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

IMPLEMENT_RULE(RuleExecuteSpy, CITIES)
