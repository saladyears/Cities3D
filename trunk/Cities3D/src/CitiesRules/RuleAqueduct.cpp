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
// Class: RuleAqueduct
// 
// Handles aqueduct usage.
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
class RuleAqueduct : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// A player can use their aqueduct only if:
		//
		// 1.  A 7 was not rolled.
		// 2.  They did not receive any resource or commodity cards.
		// 3.  They have an aqueduct (science level 3).
		//
		bool aqueductFound = false;

		wxInt32 players = numPlayers();
		wxInt32 curPlayer = current();

		bool thisPlayerAqueduct = false;
		wxString thisPlayerName;

		wxInt32 die1 = gameData<wxInt32>(shDie1);
		wxInt32 die2 = gameData<wxInt32>(shDie2);

		// Go through each player, starting with the current player.  If
		// they have an aqueduct, and received no cards this turn, 
		// stop and run the aqueduct selection dialog.
		if(7 != die1 + die2)
		{
			for(wxInt32 i = 0; i < players; ++i)
			{
				wxInt32 thisPlayer = (i + curPlayer) % players;
				wxInt32 level = playerGameData<wxInt32>(shScienceLevel, 
					thisPlayer);
				wxInt32 gained = playerGameData<wxInt32>(shResourcesGained,
					thisPlayer);

				// See if they have an aqueduct and received no cards.
				if( (3 <= level) &&
					(0 == gained))
				{
                    // This player is now in a blocking action.
                    RULE.Execute(shRuleBeginBlockingAction, DataObject(thisPlayer));

					// They have one.
					aqueductFound = true;

					// Shut everything down.
					Controller::get().Transmit(shEventBuildUI, 
						DataObject(false, GetGame()));
					Controller::get().Transmit(shEventControlsUI, 
						DataObject(false));

					Controller::get().Transmit(shEventAqueduct, 
						DataObject(thisPlayer, GetGame()));

					thisPlayerName = 
						playerGame(thisPlayer).player().Name();

					// See if this player is using their aqueduct.
					DataObject input(thisPlayer), output;
					RULE.Decide(shLogicIsThisPlayer, input, output);
					if(true == output.read<bool>())
					{
						thisPlayerAqueduct = true;
					}

					// Only do this one at a time.
					break;
				}
			}
		}

		// If all aqueducts have been used, continue on.
		if(false == aqueductFound)
		{
			RULE.Execute(shRuleMarkerPostDiceRoll, DataObject());
		}
		// Otherwise, set the text.
		else
		{
			wxString str1;

			if(true == thisPlayerAqueduct)
			{
				str1 = wxString::Format(stUseAqueduct.c_str(),
					thisPlayerName.c_str());
			}
			else
			{
				str1 = wxString::Format(stWaitingUseAqueduct.c_str(), 
					thisPlayerName.c_str());
			}

			Controller::get().Transmit(shEventMessageUI, DataObject(str1, 
				GetGame()));
		}
	}
};

IMPLEMENT_RULE(RuleAqueduct, CITIES)
