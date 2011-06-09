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
#include "Logic.h"
#include "RuleSetDatabase.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicCardListDevCard
// 
// Provides a list of the dev cards the player has, for the UI.
//
// Derived From:
//     <Logic>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <DevCards>
//
// Mixin To:
//     <LogicCardList>
//
class LogicCardListDevCard : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		wxASSERT(2 <= input.numItems());

		wxInt32 index = input.read<wxInt32>(1);

		// See if it's this player.
		RULE.Decide(shLogicIsThisPlayer, DataObject(index), output);

		if(false == output.read<bool>())
		{
			// Not their cards.
			output = DataObject(false);
		}
		else
		{
			StringArray playableCards;
			StringArray purchasedCards;

			// Create a list of translated card names.
			const Data::IntHash& playable = 
				playerGameData<Data::IntHash>(shPlayableCards, index);
			Data::IntHash::const_iterator it, itEnd = playable.end();
			for(it = playable.begin(); it != itEnd; ++it)
			{
				const HashString& card = it->first;
				wxInt32 amount = it->second;

				output.reset();
				RULE.Decide(shLogicCardName, DataObject(card), output);
				const wxString& name = output.read<wxString>();

				for(wxInt32 i = 0; i < amount; ++i)
				{
					playableCards.push_back(name);
				}
			}

			// Purchased cards.
			const Data::IntHash& purchased = 
				playerGameData<Data::IntHash>(shPurchasedCards, index);
			itEnd = purchased.end();
			for(it = purchased.begin(); it != itEnd; ++it)
			{
				const HashString& card = it->first;
				wxInt32 amount = it->second;

				output.reset();
				RULE.Decide(shLogicCardName, DataObject(card), output);
				const wxString& name = output.read<wxString>();

				for(wxInt32 i = 0; i < amount; ++i)
				{
					purchasedCards.push_back(name);
				}
			}

			output = DataObject(true, playableCards, purchasedCards);
		}
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardListDevCard, DevCard, LogicCardList, 
							DEVCARDS);
