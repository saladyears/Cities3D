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
#include "IUndoable.h"
#include "RuleSetDatabase.h"
#include "RNG.h"
#include "Controller.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleBuyDevCardTurn
// 
// Mixin for handling a click on the dev card <BuildUI> in the game state of 
// Turn, allowing the player to buy a new dev card
//
// Derived From:
//     <Rule>
//     <IUndoable>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <DevCards>
//
// Mixin To:
//     <RuleBuyDevCard>
//
// Rules Called:
//     <RuleUpdateMessageUI>
//
class RuleBuyDevCardTurn 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &)
	{
		// First, make sure we have all the resources needed to do this.
		RULE.Execute(shRuleExecuteAutoTrade, DataObject(
			shRuleBuyDevCardTurn, shLogicCostDevCard));

		// Check to see if manual auto-trade is turned on.
		if(false == gameData<HashString>(shAutoTradeRule).empty())
		{
			return;
		}

		wxInt32& bankDev = gameData<Data::IntHash>(shDevCards)[shDev];

		// Ok, so what we're going to do is put all of the dev cards in an
		// array, then pick a random index in the array and that's their card.
		HashStringArray cardArray;
		cardArray.reserve(bankDev);

		Data::IntHash& devCards = gameData<Data::IntHash>(shBankDevCards);
		Data::IntHash::iterator it, itEnd = devCards.end();
		for(it = devCards.begin(); it != itEnd; ++it)
		{
			const HashString& card = it->first;
			wxInt32 amount = it->second;
			for(int i = 0; i < amount; ++i)
			{
				cardArray.push_back(card);
			}
		}

		// Sort the cards.
		std::sort(cardArray.begin(), cardArray.end());

		size_t numCards = cardArray.size();
		size_t index = RAND.pooled(numCards);

		const HashString& thisCard = cardArray[index];

		// Victory points are immediately playable, otherwise, they are added
		// to the purchased cards.
		if(shVictoryPoint == thisCard)
		{
			playerGameData<Data::IntHash>(shPlayableCards)[thisCard]++;
		}
		else
		{
			playerGameData<Data::IntHash>(shPurchasedCards)[thisCard]++;
		}

		// Remove the card from the bank.
		devCards[thisCard]--;
		bankDev--;
		wxASSERT(0 <= devCards[thisCard]);
		wxASSERT(0 <= bankDev);

		// Adjust the resources used.
		DataObject input(0), output;
		RULE.Decide(shLogicCostDevCard, input, output);
		RULE.Execute(shRulePurchase, output);

		// Update the players.
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stBuysADevelopmentCard));

		RULE.Execute(shRulePlaySound, DataObject(SOUND_BUY_DEV_CARD));

		// Tell just this player what the card is.
		output.reset();
		input = DataObject(-1);
		RULE.Decide(shLogicIsThisPlayer, input, output);
		if(true == output.read<bool>())
		{
			output.reset();
			RULE.Decide(shLogicCardName, DataObject(thisCard), output);
			const wxString& name = output.read<wxString>();
			
			wxString str = wxString::Format(stYouReceiveACard.c_str(), 
				name.c_str());
			RULE.AsynchExecute(shRuleUpdateNetworkUI, DataObject(str));
		}

		// Adjust stats.
		RULE.Execute(shRulePurchaseItem, DataObject(current(), shDevCard));

		// Back to turn actions.
		RULE.Execute(shRuleRestartTurn, DataObject());

		// Buying a dev card causes a game save.
		RULE.Execute(shRuleAutoSave, DataObject());

		Controller::get().Transmit(shEventDirty, 0);

        // The user performed a game action.
        RULE.Execute(shRulePerformGameAction, DataObject());
	}

	virtual void Unexecute(const DataObject&)
	{
		wxASSERT(false);
	}

	virtual bool CanUndo() const
	{
		// Buying a dev card can never be undone.
		return false;
	}

private:
};

IMPLEMENT_RULE_KEYED_MIXIN(RuleBuyDevCardTurn, Turn, RuleBuyDevCard, DEVCARDS)
