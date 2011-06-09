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
// Class: RulePurchase
// 
// Makes a purchase for a player, adjusting the resource totals of them and
// the bank.  Also updates the UI appropriately.
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
//
class RulePurchase 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		typedef std::map<HashString, wxInt32> ResourceMap;
		const ResourceMap &resources = object.read<ResourceMap>();

		ResourceMap::const_iterator it, itEnd = resources.end();
		for(it = resources.begin(); it != itEnd; ++it)
		{
			const HashString &resource = it->first;
			const wxInt32 amount = it->second;

			// RuleBankTransact works by adding to the player and subtracting
			// from the bank, so use the negative amount.
			DataObject thisObject(current(), resource, -amount);
			
			// Adjust resources.
			RULE.Execute(shRuleBankTransact, thisObject);
		}

		// Update the UI.
		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), -1));
	}

	virtual void Unexecute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		typedef std::map<HashString, wxInt32> ResourceMap;
		const ResourceMap &resources = object.read<ResourceMap>();

		ResourceMap::const_iterator it, itEnd = resources.end();
		for(it = resources.begin(); it != itEnd; ++it)
		{
			const HashString &resource = it->first;
			const wxInt32 amount = it->second;

			DataObject thisObject(current(), resource, amount);
			
			// Adjust resources.
			RULE.Execute(shRuleBankTransact, thisObject);
		}

		// Update the UI.
		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), -1));
	}

	virtual bool CanUndo() const
	{
		// Purchases should never stop an undo chain.
		return true;
	}
};

IMPLEMENT_RULE(RulePurchase, STANDARD)
