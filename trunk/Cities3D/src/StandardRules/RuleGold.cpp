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
// Class: RuleGold
// 
// Handles gold.
//
// Derived From:
//     <Rule>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <Seafarers>
//
class RuleGold : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		bool goldFound = false;

		wxInt32 players = numPlayers();
		wxInt32 curPlayer = current();

		bool thisPlayerGold = false;
		wxString thisPlayerName;

		// Go through each player, starting with the current player.  If
		// they have gold, stop and run the gold selection dialog.
		for(wxInt32 i = 0; i < players; ++i)
		{
			wxInt32 thisPlayer = (i + curPlayer) % players;

			const Data::IntHash& resources =
				playerGameData<Data::IntHash>(shResources, thisPlayer);

			// See if they have gold.
			Data::IntHash::const_iterator it = resources.find(shGold);

			if( (resources.end() != it) &&
				(0 < it->second))
			{
				// They have some.
				goldFound = true;

				// This player is now in a blocking action.
				RULE.Execute(shRuleBeginBlockingAction, DataObject(thisPlayer));

				// Shut everything down.
				Controller::get().Transmit(shEventBuildUI, 
					DataObject(false, GetGame()));
				Controller::get().Transmit(shEventControlsUI, 
					DataObject(false));

				Controller::get().Transmit(shEventGold, 
					DataObject(thisPlayer, GetGame()));

				thisPlayerName = playerGame(thisPlayer).player().Name();

				// See if this player is spending gold.
				DataObject input(thisPlayer), output;
				RULE.Decide(shLogicIsThisPlayer, input, output);
				if(true == output.read<bool>())
				{
					thisPlayerGold = true;
				}

				// Only do this one at a time.
				break;
			}
		}

		// If all gold has been spent, continue on.
		if(false == goldFound)
		{
			RULE.Execute(shRuleMarkerPostDiceRoll, DataObject());
		}
		// Otherwise, set the text.
		else
		{
			wxString str1;

			if(true == thisPlayerGold)
			{
				str1 = wxString::Format(stStringSpendYourGold.c_str(),
					thisPlayerName.c_str());
			}
			else
			{
				str1 = wxString::Format(stWaitingSpendGold.c_str(),
					thisPlayerName.c_str());
			}

			Controller::get().Transmit(shEventMessageUI, DataObject(str1, 
				GetGame()));
		}
	}
};

IMPLEMENT_RULE(RuleGold, GOLD)
