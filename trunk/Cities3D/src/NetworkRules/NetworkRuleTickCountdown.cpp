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
#include "NetworkPlayers.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkRuleTickCountdown
// 
// Ticks one second off each valid player countdown.
//
// Derived From:
//     <Rule>
//
// Project:
//     <NetworkRules>
//
// RuleSet:
//     <Network>
//
class NetworkRuleTickCountdown : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// Look for players with valid countdowns.
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			wxTimeSpan &time = playerGameData<wxTimeSpan>(shRestartTime, i);
			if(-1 != time.GetMilliseconds())
			{
				// Subtract one second and update the UI.
				if(0 < time.GetValue())
				{
					time -= wxTimeSpan(0, 0, 1);
				}

				Controller::get().Transmit(shEventCountdownTime, 
					DataObject(i, time));
			}
		}

		// Check to see if there's any spots now free.
		RULE.Execute(shNetworkRuleCheckCountdowns, DataObject());
	}
};

IMPLEMENT_RULE(NetworkRuleTickCountdown, NETWORK)
