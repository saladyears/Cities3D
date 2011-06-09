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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkRuleStartCountdown
// 
// Sets a wxTimeSpan with the given time amount for the given color, if one
// hasn't been started already.
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
class NetworkRuleStartCountdown : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		const ColorType color = object.read<ColorType>();
		const size_t seconds = object.read<size_t>(1);

		// Find this color and set their timer, if it doesn't exist
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			wxTimeSpan &time = playerGameData<wxTimeSpan>(shRestartTime, i);

			if( (color == playerGame(i).color()) &&
				(-1 == time.GetMilliseconds()))
			{
				time = wxTimeSpan(0, 0, seconds);
				break;
			}
		}
	}
};

IMPLEMENT_RULE(NetworkRuleStartCountdown, NETWORK)
