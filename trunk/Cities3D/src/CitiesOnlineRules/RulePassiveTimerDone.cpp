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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePassiveTimerDone
// 
// Handles a player running out of passive quit time.
//
// Derived From:
//     <Rule>
//
// Project:
//     <CitiesOnlineRules>
//
// RuleSet:
//     <CitiesOnline>
//
class RulePassiveTimerDone : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
        wxInt32 actualIndex = object.read<wxInt32>();

        // If they are on the passive counter, they're done.  We disconnect
        // them with a passive quit.    
        if(TRUE == playerGameData<wxInt32>(shPassiveTurnTimer, actualIndex))
        {
            playerGameData<wxInt32>(shPassiveQuit, actualIndex) = TRUE;

            // Run the quit rule
            RULE.Execute(shNetworkRuleQuit, 
                DataObject(playerGame(actualIndex).player()));

            // If we're the player at this computer, quit the game.
            DataObject input(actualIndex), output;
            RULE.Decide(shLogicIsThisPlayer, input, output);

            if(true == output.read<bool>())
            {
                Controller::get().Transmit(shEventExit, 
                    static_cast<wxWindow*>(NULL));
            }
            // Otherwise, play the sound.
            else
            {
                RULE.Execute(shRulePlaySound, DataObject(SOUND_PASSIVE_QUIT));
            }
        }
	}
};

IMPLEMENT_RULE(RulePassiveTimerDone, CITIESONLINE)
