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
#include "NetworkPlayers.h"
#include "HTML.h"
#include "Controller.h"
#include "INetworkEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkRuleCheckCountdowns
// 
// Checks all countdowns to see if a color has opened up for a spectator to
// join.
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
// Rules Called:
//     <NetworkRuleSpectatorReplace>
//
class NetworkRuleCheckCountdowns : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// Only do stuff if this is the host.
		if(true == NET.Players().IsHost())
		{
			for(wxInt32 i = 0; i < numPlayers(); ++i)
			{
				wxTimeSpan &time = playerGameData<wxTimeSpan>(shRestartTime, i);

				if(0 == time.GetValue())
				{
					// We've got one at zero, see if there's a spectator to
					// replace them.
					const SpectatorArray &spectators = 
						NET.Players().GetSpectators();

					// Search for the first one with a want to play flag.
					SpectatorArray::const_iterator it, 
						itEnd = spectators.end();
					for(it = spectators.begin(); it != itEnd; ++it)
					{
						if(TRUE == it->second)
						{
							ColorType color = playerGame(i).color();

							wxString str = wxString::Format(
								stTakesOverAs.c_str(), 
								it->first.Name().c_str(),
								ColorInfoData::Name(color).c_str());
							HTML::Color(str, color);

							// We've got one.  Slap 'em in.
							NET.SendRule(shNetworkRulePlayerReplace,
								DataObject(it->first, i, str));
							break;
						}
					}
				}
			}
		}
	}
};

IMPLEMENT_RULE(NetworkRuleCheckCountdowns, NETWORK)
