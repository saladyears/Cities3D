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
#include "RNG.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleExecuteRobberSteal
// 
// Steals one resource randomly from the given player.
//
// Derived From:
//     <Rule>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <DevCards>
//
class RuleExecuteRobberSteal : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		wxInt32 stealPlayer = object.read<wxInt32>();

		// Gather up all the player's resources.
		HashStringArray steal;
		const Data::IntHash& resources = 
			playerGameData<Data::IntHash>(shResources, stealPlayer);
		Data::IntHash::const_iterator it, itEnd = resources.end();
		for(it = resources.begin(); it != itEnd; ++it)
		{
			const HashString& res = it->first;
			wxInt32 amount = it->second;
			
			for(wxInt32 i = 0; i < amount; ++i)
			{
				steal.push_back(res);
			}
		}

		// Need to sort to ensure cross platform consistency.
		std::sort(steal.begin(), steal.end());

		wxASSERT(false == steal.empty());

		// Now pick a random resource out.
		size_t index = RAND.pooled(steal.size());
		const HashString& res = steal[index];

		// Adjust the totals for each player.
		RULE.Execute(shRuleAdjustResources, DataObject(stealPlayer, -1, res));
		RULE.Execute(shRuleAdjustResources, DataObject(current(), 1, res));
		
		// Update their UI.
		Controller::get().Transmit(shEventPlayerUI, DataObject(GetGame(), -1));

		// There's a chance the player may have gone to zero so redraw the
		// screen.
		Controller::get().Transmit(shEventDirty, 0);
		
		// Put out a UI message, which depends on who is involved.

		DataObject input(res), output;
		RULE.Decide(shLogicResourceName, input, output);
		wxString resName = output.read<wxString>();

		const wxString &playerName = playerGame(stealPlayer).player().Name();

		// This Player.
		wxString str;
		input = DataObject(-1);
		output.reset();
		RULE.Decide(shLogicIsThisPlayer, input, output);
		bool thisPlayer = output.read<bool>();

		// The other player.
		input = DataObject(stealPlayer);
		output.reset();
		RULE.Decide(shLogicIsThisPlayer, input, output);
		bool otherPlayer = output.read<bool>();

		if(true == thisPlayer)
		{
			str = wxString::Format(stYouStealOne.c_str(),
				resName.c_str(), playerName.c_str());
		}
		else if(true == otherPlayer)
		{
			str = wxString::Format(stStealsOneFromYou.c_str(),
				swStringFormat.c_str(), resName.c_str());
		}
		else
		{
			str = wxString::Format(stStealsOneResourceFrom.c_str(),
				swStringFormat.c_str(), playerName.c_str());
		}

		RULE.Execute(shRuleUpdateNetworkUI, DataObject(str));

		RULE.Execute(shRulePlaySound, DataObject(SOUND_STEAL_CARD));
	}
};

IMPLEMENT_RULE(RuleExecuteRobberSteal, STANDARD)
