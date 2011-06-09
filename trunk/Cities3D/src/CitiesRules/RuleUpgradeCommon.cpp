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
// Class: RuleUpgradeCommon
// 
// Handles any city upgrade.
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
// Rules Called:
//     <RuleUpdateMessageUI>
//
class RuleUpgradeCommon 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		const HashString& type = object.read<HashString>();
		HashString rule = HashString::Format(shRuleUpgradeString.c_str(), 
            type.c_str());
		HashString logic = HashString::Format(shLogicCostUpgradeString.c_str(), 
			type.c_str());

		// First, make sure we have all the resources needed to do this.
		RULE.Execute(shRuleExecuteAutoTrade, DataObject(rule, logic, false));

		// Check to see if manual auto-trade is turned on.
		if(false == gameData<HashString>(shAutoTradeRule).empty())
		{
			return;
		}

		// Adjust the resources used.
		DataObject input(0), output;
		RULE.Decide(logic, input, output);
		RULE.Execute(shRulePurchase, output);

		// If it was reduced price, tick it off.
		wxBOOL builtFree = FALSE;

		wxInt32 &free = playerGameData<wxInt32>(shFreeCityUpgrades);
		if(0 < free)
		{
			builtFree = TRUE;
			--free;
		}

		playerGameData<wxInt32>(shCityUpgradeFree) = builtFree;

		// Now that the player is guaranteed to have the resources, upgrade
		// their type.
		HashString key = HashString::Format(shStringLevel.c_str(), type.c_str());
		wxInt32& level = playerGameData<wxInt32>(key);
		wxASSERT(5 > level);

		++level;

		input = DataObject(type);
		output.reset();
		RULE.Decide(shLogicUpgradeName, input, output);
		const wxString& name = output.read<wxString>();

		// Update the UI.
		wxString str = wxString::Format(stUpgradesToLevel.c_str(), 
			swStringFormat.c_str(), name.c_str(), level);
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(str));

		// They may be allowed to build a mighty knight now so refresh the
		// BuildUI.
		Controller::get().Transmit(shEventCityUpgrade, GetGame());
		Controller::get().Transmit(shEventBuildUI, DataObject(true, 
			GetGame()));

		gameData<HashString>(shState) = shTurn;
		
		// Check for a metropolis.
		RULE.Execute(shRuleMetropolis, object);

		// Back to turn actions (possibly).
		RULE.Execute(shRuleRestartTurn, DataObject());
	}

	virtual void Unexecute(const DataObject &object)
	{
		const HashString& type = object.read<HashString>();
		
		HashString key = HashString::Format(shStringLevel.c_str(), type.c_str());
		wxInt32& level = playerGameData<wxInt32>(key);
		wxASSERT(0 < level);

		--level;

		// Update the UI.
		wxString str = wxString::Format(stUndoesTheUpgrade.c_str(), 
            swStringFormat.c_str(), type.c_str());
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(str));

		if(TRUE == playerGameData<wxInt32>(shCityUpgradeFree))
		{
			wxInt32 &free = playerGameData<wxInt32>(shFreeCityUpgrades);
			++free;
		}

		Controller::get().Transmit(shEventCityUpgrade, GetGame());
		Controller::get().Transmit(shEventBuildUI, DataObject(true, 
			GetGame()));
	}

	virtual bool CanUndo() const
	{
		return true;
	}
};

IMPLEMENT_RULE(RuleUpgradeCommon, CITIES)
