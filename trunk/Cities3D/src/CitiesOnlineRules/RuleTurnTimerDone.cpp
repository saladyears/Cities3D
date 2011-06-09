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
// Class: RuleTurnTimerDone
// 
// Handles a player running out of turn time.
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
class RuleTurnTimerDone : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
        // If they have extra turn times to burn, we burn them.
        wxInt32& extraTimers = playerGameData<wxInt32>(shExtraTimers);

        wxInt32& timerStep = playerGameData<wxInt32>(shExtraTimerStep);
        wxInt32 timers = extraTimers - timerStep;
        extraTimers = std::max(0, extraTimers - timerStep);
        
        if(0 <= timers)
        {
            wxString str;

            if(1 == timerStep)
            {
                str = wxString::Format(stUsesOneTurnTimer.c_str(), 
                    swStringFormat.c_str(), extraTimers);
            }
            else
            {
                str = wxString::Format(stUsesXTurnTimers.c_str(), 
                    swStringFormat.c_str(), timerStep, extraTimers);
            }

            // Make a note of what happened.
            RULE.Execute(shRuleUpdateNetworkUI, DataObject(str));

            // Play a sound.
            RULE.Execute(shRulePlaySound, DataObject(SOUND_USE_ONE_TURN_TIMER));

            ++timerStep;

            // Restart the countdown.
            gameData<wxInt32>(shIgnoreExtraTimers) = TRUE;
            RULE.Execute(shRuleResetTurnTime, DataObject(true));
            gameData<wxInt32>(shIgnoreExtraTimers) = FALSE;
        }
        // Otherwise, they are out of extra timers.  We move them 
        // forward by ending the turn.
        else
        {
            // Play a sound.
            RULE.Execute(shRulePlaySound, DataObject(SOUND_OUT_OF_TIME));

            // Make a note of what happened.
            RULE.Execute(shRuleUpdateNetworkUI, DataObject(stIsOutOfTime));
         
            // End the turn.
            RULE.Execute(shRuleMarkerEndTurn, DataObject());
        }
	}
};

IMPLEMENT_RULE(RuleTurnTimerDone, CITIESONLINE)
