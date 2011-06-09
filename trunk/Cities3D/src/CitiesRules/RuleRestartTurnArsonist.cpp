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
// Class: RuleRestartTurnArsonist
// 
// Restarts the game from players losing cards to an arsonist
//
// Derived From:
//     <Rule>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// Mixin To:
//     <RuleRestartGame>
//
class RuleRestartTurnArsonist : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
        // This isn't purely accurate, but the current player is now 100%
        // dependent on the other players finishing and should not have a
        // turn timer.
        RULE.Execute(shRuleEndBlockingAction, DataObject(current()));

		bool done = true;
		bool first = true;
		bool thisPlayer = false;
		wxString thisName;

		// If there's no data in the object, this is the first time through,
		// so show the lose card dialogs.
		bool showDialog = true;

		if(1 <= object.numItems())
		{
			showDialog = object.read<bool>();
		}

		wxString str;

		// See if any players need to lose cards.
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			if(TRUE == playerGameData<wxInt32>(shArsonist, i))
			{
				if(true == showDialog)
				{
                    // This player is now in a blocking action.
                    RULE.Execute(shRuleBeginBlockingAction, DataObject(i));

					// Shut everything down.
					Controller::get().Transmit(shEventBuildUI, 
						DataObject(false, GetGame()));
					Controller::get().Transmit(shEventControlsUI, 
						DataObject(false));

					Controller::get().Transmit(shEventArsonist, 
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
			RULE.Execute(shRulePlayProgressCardEnd, DataObject());
		}
		else
		{
			wxString str1;

			if(true == thisPlayer)
			{
				str1 = wxString::Format(stSelectResourcesToLose.c_str(),
					thisName.c_str());
			}
			else
			{
				str1 = wxString::Format(stWaitingLoseResources.c_str(),
					str.c_str());
			}

			Controller::get().Transmit(shEventMessageUI, DataObject(str1, 
				GetGame()));
		}
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RuleRestartTurnArsonist, TurnArsonist, 
						    RuleRestartGame, CITIES)
