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
#include "IUndoable.h"
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
// Class: RuleExecuteBankTrade
// 
// Executes a bank trade for the current player.
//
// Derived From:
//     <Rule>
//     <IUndoable>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// Transmits To:
//     eventPlayerUI
//     eventBuildUI
//
class RuleExecuteBankTrade 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(2 <= object.numItems());

		typedef std::map<HashString, wxInt32> ResourceMap;
		const ResourceMap& player = object.read<ResourceMap>();
		const ResourceMap& bank = object.read<ResourceMap>(1);

		wxInt32 curPlayer = current();

		wxString str1;

		// Subtract the player's side from the player and add it to the bank.
		ResourceMap::const_iterator it, itEnd = player.end();
		for(it = player.begin(); it != itEnd; ++it)
		{
			const HashString& res = it->first;
			const wxInt32 amount = it->second;

			RULE.Execute(shRuleBankTransact, DataObject(curPlayer, res, 
				-amount));
			if(it != player.begin())
			{
				str1 += swCommaSpace;
			}

			// Get the translated name of the resource.
			DataObject input(res), output;
			RULE.Decide(shLogicResourceName, input, output);
			const wxString& name = output.read<wxString>();

			str1 += wxString::Format(swIntString.c_str(), amount, name.c_str());
		}

		wxString str2;

		// Add the bank's side to the player and subtract it from the bank.
		itEnd = bank.end();
		for(it = bank.begin(); it != itEnd; ++it)
		{
			const HashString& res = it->first;
			const wxInt32 amount = it->second;

			RULE.Execute(shRuleBankTransact, DataObject(curPlayer, res, 
				amount));

			if(it != bank.begin())
			{
				str2 += swCommaSpace;
			}

			// Get the translated name of the resource.
			DataObject input(res), output;
			RULE.Decide(shLogicResourceName, input, output);
			const wxString& name = output.read<wxString>();

			str2 += wxString::Format(swIntString.c_str(), amount, name.c_str());
		}

		wxString str = wxString::Format(stTradesToTheBank.c_str(),
			swStringFormat.c_str(), str1.c_str(), str2.c_str());

		// Update the player and their build ui.
		Controller::get().Transmit(shEventPlayerUI, DataObject(GetGame(), 
			curPlayer));

		RULE.Execute(shRuleUpdateNetworkUI, DataObject(str));

		RULE.Execute(shRulePlaySound, DataObject(SOUND_TRADE_BANK));

		// If, after executing the trade, the player can't make any more 
		// trades, close any  UI, as long as we're not in auto-trade mode.
		if(true == gameData<HashString>(shAutoTradeRule).empty())
		{
			Controller::get().Transmit(shEventBuildUI, DataObject(true, 
				GetGame()));

			DataObject input(0), output;
			RULE.Decide(shLogicCanTradeBank, input, output);
			if(false == output.read<bool>())
			{
				// Shut down all modeless dialog boxes.
				Controller::get().Transmit(shEventShutdownModelessUI, 0);
			}
		}
	}

	void Unexecute(const DataObject& object)
	{
		wxASSERT(2 <= object.numItems());

		typedef std::map<HashString, wxInt32> ResourceMap;
		const ResourceMap& player = object.read<ResourceMap>();
		const ResourceMap& bank = object.read<ResourceMap>(1);

		wxInt32 curPlayer = current();

		// Subtract the player's side from the bank and add it back to the 
		// player.
		ResourceMap::const_iterator it, itEnd = player.end();
		for(it = player.begin(); it != itEnd; ++it)
		{
			RULE.Execute(shRuleBankTransact, DataObject(curPlayer, it->first, 
				it->second));
		}

		// Add the bank's side to the bank and subtract it from the player.
		itEnd = bank.end();
		for(it = bank.begin(); it != itEnd; ++it)
		{
			RULE.Execute(shRuleBankTransact, DataObject(curPlayer, it->first, 
				-(it->second)));
		}

		// Update the player and their build ui.
		Controller::get().Transmit(shEventPlayerUI, DataObject(GetGame(), 
			curPlayer));
		Controller::get().Transmit(shEventBuildUI, DataObject(true, 
			GetGame()));

		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stUndoesABankTrade));
	}

	bool CanUndo() const
	{
		// Bank trades can always be undone.
		return true;
	}
};

IMPLEMENT_RULE(RuleExecuteBankTrade, STANDARD)
