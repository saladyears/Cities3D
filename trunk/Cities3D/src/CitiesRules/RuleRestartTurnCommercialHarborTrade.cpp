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
// Class: RuleRestartTurnCommercialHarborTrade
// 
// Restarts the game from the current player and the commercial harbor victim
// trading cards.
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
// Mixin To:
//     <RuleRestartGame>
//
class RuleRestartTurnCommercialHarborTrade : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		bool showDialogs = true;

		if(1 <= object.numItems())
		{
			showDialogs = object.read<bool>();
		}

		// Here's how this works.  Both the current player and the commercial
		// harbor victim select their resource and commodity simultaneously.
		// Only when both cards have been selected do we move on.
		const HashString& res = gameData<HashString>(shCommercialHarborResource);
		const HashString& com = gameData<HashString>(shCommercialHarborCommodity);

		// If both cards are set, move on.
		if( (false == res.empty()) &&
			(false == com.empty()))
		{
			RULE.Execute(shRuleExecuteCommercialHarbor, DataObject(res, com));
		}
		else
		{
			const wxString& name = playerGame(current()).player().Name();

			wxInt32 victim = gameData<wxInt32>(shCommercialHarborVictim);
			
            if(true == showDialogs)
            {
                // These players are now in a blocking action.
                RULE.Execute(shRuleBeginBlockingAction, DataObject(current()));
                RULE.Execute(shRuleBeginBlockingAction, DataObject(victim));
            }

			const wxString& victimName = playerGame(victim).player().Name();
            wxString str = wxString::Format(
				stWaitingToTradeUsingCommercialHarbor.c_str(),
				name.c_str(), victimName.c_str());

			if(true == res.empty())
			{
				DataObject input(-1), output;
				RULE.Decide(shLogicIsThisPlayer, input, output);
				if(true == output.read<bool>())
				{
					if(true == showDialogs)
					{
						Controller::get().Transmit(
							shEventCommercialHarborResource, GetGame());
					}

					str = wxString::Format(
						stSelectAResourceToGive.c_str(), 
						name.c_str(), victimName.c_str());
				}
				else if(false == com.empty())
				{
					str = wxString::Format(
						stWaitingSelectAResource.c_str(),
						name.c_str());
				}
			}

			if(true == com.empty())
			{
				DataObject input(victim), output;
				RULE.Decide(shLogicIsThisPlayer, input, output);
				if(true == output.read<bool>())
				{
					if(true == showDialogs)
					{
						Controller::get().Transmit(
							shEventCommercialHarborCommodity, GetGame());
					}

					str = wxString::Format(
						stSelectACommodityToGive.c_str(), 
						victimName.c_str(), name.c_str());
				}
				else if(false == res.empty())
				{
					str = wxString::Format(
						stWaitingSelectACommodity.c_str(),
						victimName.c_str());
				}
			}

			Controller::get().Transmit(shEventMessageUI, DataObject(str, 
				GetGame()));

			Controller::get().Transmit(shEventControlsUI, DataObject(false));
			Controller::get().Transmit(shEventBuildUI, 
				DataObject(false, GetGame()));
		}
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RuleRestartTurnCommercialHarborTrade, 
						   TurnCommercialHarborTrade, RuleRestartGame, CITIES)
