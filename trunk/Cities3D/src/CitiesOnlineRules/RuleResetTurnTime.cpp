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
// Class: RuleResetTurnTime
// 
// Does stuff.
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
class RuleResetTurnTime : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		DataObject input(-1), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);

        bool restart = object.read<bool>();
            
        // Ensure that if the player is on a turn timer, they are never marked 
        // as being on a passive timer.  This can happen in the PreRoll state, 
        // since we move directly from passive to turn.
        if(true == restart)
        {
            playerGameData<wxInt32>(shPassiveTurnTimer) = FALSE;
        }

		if(true == output.read<bool>())
		{
            wxInt32 seconds = -1;
            wxInt32 timers = 0;
            wxInt32 step = 0;

            if(true == restart)
            {
                timers = playerGameData<wxInt32>(shExtraTimers);
                wxInt32& timerStep = playerGameData<wxInt32>(shExtraTimerStep);

                // Reset their timer step count back to 1, unless it's just been
                // incremented.
                if(FALSE == gameData<wxInt32>(shIgnoreExtraTimers))
                {
                    timerStep = 1;
                }

			    seconds = gameData<wxInt32>(shTurnTime);
                step = timerStep;
            }

			Controller::get().Transmit(shEventResetTurnTime, 
                DataObject(seconds, timers, step));
		}
        else
        {
            Controller::get().Transmit(shEventResetTurnTime, 
                DataObject(-1, 0, 0));
        }
	}
};

IMPLEMENT_RULE(RuleResetTurnTime, CITIESONLINE)
