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
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRestartDiscardProgressCards
// 
// Restarts the game from players discarding a progress card.
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
class RuleRestartDiscardProgressCards : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		bool done = true;
		bool first = true;
		bool thisPlayer = false;
		wxString thisName;

		// If there's no data in the object, this is the first time through,
		// so show the dialogs.
		bool showDialog = true;
		wxInt32 player = -1;

		if(1 <= object.numItems())
		{
			// It's possible for a player to have 6 cards, if they get 5 on 
			// their turn, don't use any, then get another on the next roll.
			// In that case, they may have to discard again.
			player = object.read<wxInt32>();
		}

		wxString str;
		wxInt32 curPlayer = current();

		// See if any players need to discard a progress card.
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			if( (i != curPlayer) &&
				(4 < FindTotal(i)))
			{
				if( (true == showDialog) ||
					(player == i))
				{
                    // This player is now in a blocking action.
                    RULE.Execute(shRuleBeginBlockingAction, DataObject(curPlayer));

					// Shut everything down.
					Controller::get().Transmit(shEventBuildUI, 
						DataObject(false, GetGame()));
					Controller::get().Transmit(shEventControlsUI, 
						DataObject(false));

					Controller::get().Transmit(shEventDiscardProgressCards, 
						DataObject(i, GetGame()));
				}

				// Add their name to the waiting list.
				if(true == first)
				{
					first = false;
				}
				else
				{
					str += swCommaSpace;
				}

				const wxString& name = playerGame(i).player().Name();
				str += name;

				// See if this player is losing cards.
				DataObject input(i), output;
				RULE.Decide(shLogicIsThisPlayer, input, output);
				if(true == output.read<bool>())
				{
					thisPlayer = true;
					thisName = name;
				}

				done = false;
			}
		}

		if(true == done)
		{
			RULE.Execute(shRuleMarkerPostDiceRoll, DataObject());
		}
		else
		{
			wxString str1;

			if(true == thisPlayer)
			{
				str1 = wxString::Format(stDiscardOneProgressCard.c_str(),
					thisName.c_str());
			}
			else
			{
				str1 = wxString::Format(stWaitingDiscardOneProgressCard.c_str(),
					str.c_str());
			}

			Controller::get().Transmit(shEventMessageUI, DataObject(str1, 
				GetGame()));
		}
	}

private:
	wxInt32 FindTotal(wxInt32 player)
	{
		wxInt32 total = 0;
		total += Sum(shPlayableTradeCards, player);
		total += Sum(shPlayableScienceCards, player);
		total += Sum(shPlayablePoliticsCards, player);

		return total;
	}

	wxInt32 Sum(const HashString& type, wxInt32 player)
	{
		wxInt32 total = 0;

		const Data::IntHash& cards = playerGameData<Data::IntHash>(type, 
			player);
		Data::IntHash::const_iterator it, itEnd = cards.end();
		for(it = cards.begin(); it != itEnd; ++it)
		{
			total += it->second;
		}

		return total;
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RuleRestartDiscardProgressCards, 
						   DiscardProgressCards, RuleRestartGame, CITIES)
