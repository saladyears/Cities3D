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
// Class: RuleExecuteAutoTrade
// 
// Auto-trades for a player if needed.
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
//
class RuleExecuteAutoTrade 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(2 <= object.numItems());

		bool breakUndo = true;

		const HashString& rule = object.read<HashString>();
		const HashString& logic = object.read<HashString>(1);

		if(3 <= object.numItems())
		{
			breakUndo = object.read<bool>(2);
		}

		// Find out how much this thing costs.
		DataObject input(0), output;
		RULE.Decide(logic, input, output);

		// Now that we've got the cost, run the auto-trade logic.
		DataObject thisOutput;
		RULE.Decide(shLogicCanAutoTrade, output, thisOutput);

		typedef std::map<HashString, wxInt32> ResourceMap;
		typedef std::pair<ResourceMap, ResourceMap> ResourcePair;

		wxInt32 tradesNeeded = thisOutput.read<wxInt32>();
		wxInt32 tradesInHand = thisOutput.read<wxInt32>(1);
		const ResourcePair& pair = thisOutput.read<ResourcePair>(2);
		const ResourceMap& resourcesNeeded = pair.first;
		const ResourceMap& resourcesInHand = pair.second;

		// If we just completed an auto-trade, don't break undo.
		if(TRUE == gameData<wxInt32>(shAutoTradeComplete))
		{
			breakUndo = false;
			gameData<wxInt32>(shAutoTradeComplete) = FALSE;
		}

		// If we have needed resources, and we have the exact same amount of
		// resources in hand to do it, execute the trade.  If we have more than
		// enough resources to trade, we enter the auto trade state where the
		// UI must be run for the user to choose what they want to trade.
		if(0 < tradesNeeded)
		{
			if(tradesInHand == tradesNeeded)
			{
				// Run the auto-trades.  It doesn't really matter what order
				// they get traded in, so just go through both lists at the
				// same time.

				// Ha, all we have to do is just send them all down to the
				// bank execute rule.
				RULE.Execute(shRuleExecuteBankTrade, DataObject(
					resourcesInHand, resourcesNeeded));

				// This should not break undoability.
				breakUndo = false;
			}
			else if(tradesInHand > tradesNeeded)
			{
				// Manual auto-trade.
				gameData<HashString>(shAutoTradeLogic) = logic;
				gameData<HashString>(shAutoTradeRule) = rule;

				Controller::get().Transmit(shEventBankTrade, GetGame());

				// Manual auto-trades should not turn on undo.
				
			}
			else
			{
				// This should never happen.
				wxASSERT(false);
			}
		}

		if(true == breakUndo)
		{
			RULE.Execute(shRuleBreakUndo, DataObject());
		}
	}

	virtual void Unexecute(const DataObject&)
	{
		// Do nothing, the sub rules will handle their own undo.
	}

	virtual bool CanUndo() const
	{
		return true;
	}
};

IMPLEMENT_RULE(RuleExecuteAutoTrade, STANDARD)
