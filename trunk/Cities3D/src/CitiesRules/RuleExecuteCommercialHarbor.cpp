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
#include "IRuleEngine.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleExecuteCommercialHarbor
// 
// Handles the player selecting who to use the master merchant on.
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
class RuleExecuteCommercialHarbor : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(2 <= object.numItems());

		const HashString& res = object.read<HashString>();
		const HashString& com = object.read<HashString>(1);

		wxInt32 currentIndex = current();
		wxInt32 victimIndex = gameData<wxInt32>(shCommercialHarborVictim);

		// Mark this player as no longer able to trade.
		playerGameData<wxInt32>(shCommercialHarbor, victimIndex) = FALSE;

		// The current player loses the resource, the victim player gains it.
		RULE.Execute(shRuleAdjustResources, DataObject(currentIndex, -1, res));
		RULE.Execute(shRuleAdjustResources, DataObject(victimIndex, 1, res));
		
		// The current player gains the commodity, the victim player loses it.
		RULE.Execute(shRuleAdjustResources, DataObject(currentIndex, 1, com));
		RULE.Execute(shRuleAdjustResources, DataObject(victimIndex, -1, com));

		// Translate text names.
		DataObject input(res), output;
		RULE.Decide(shLogicResourceName, input, output);
		wxString resName = output.read<wxString>();

		input = DataObject(com);
		output.reset();
		RULE.Decide(shLogicResourceName, input, output);
		wxString comName = output.read<wxString>();

		input = DataObject(currentIndex);
		output.reset();
		RULE.Decide(shLogicIsThisPlayer, input, output);
		bool isCurrent = output.read<bool>();

		input = DataObject(victimIndex);
		output.reset();
		RULE.Decide(shLogicIsThisPlayer, input, output);
		bool isVictim = output.read<bool>();

		const wxString& victimName = 
			playerGame(victimIndex).player().Name();
		const wxString& currentName =
			playerGame(currentIndex).player().Name();

		wxString str;

		if(true == isCurrent)
		{
			str = wxString::Format(stYouTradeUsingTheCommercialHarbor.c_str(), 
				resName.c_str(), victimName.c_str(),
				comName.c_str());
		}
		else if(true == isVictim)
		{
			str = wxString::Format(stYouTradeUsingTheCommercialHarbor.c_str(), 
				comName.c_str(), currentName.c_str(),
				resName.c_str());
		}
		else
		{
			str = wxString::Format(stTradesOneResourceForOneCommodity.c_str(), 
				currentName.c_str(), victimName.c_str());
		}

		RULE.Execute(shRuleUpdateNetworkUI, DataObject(str));

		// Play the sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_TRADE_EXECUTE));

		// Adjust stats.
		RULE.Execute(shRuleTakeAction, DataObject(current(), shCommercialHarbor));

		// Update their player stuff.
		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), currentIndex));
		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), victimIndex));
		
		RULE.Execute(shRulePlayProgressCardEnd, DataObject());
	}
};

IMPLEMENT_RULE(RuleExecuteCommercialHarbor, CITIES)
