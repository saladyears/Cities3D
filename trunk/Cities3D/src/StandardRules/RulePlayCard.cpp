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
#include "RuleHashMixin.h"
#include "Controller.h"
#include "IRuleEngine.h"
#include "IUndoable.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePlayCard
// 
// Plays the given card.
//
// Derived From:
//     <Rule>
//     <RuleHashMixin>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
class RulePlayCard 
	: public Rule
	, public RuleHashMixin
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
		const HashString& card = object.read<HashString>();

		// Let everyone know what card they are playing.
		DataObject output;
		RULE.Decide(shLogicCardName, object, output);
		const wxString& name = output.read<wxString>();

		wxString str = wxString::Format(stPlaysACard.c_str(), 
            swStringFormat.c_str(), name.c_str());
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(str));

		bool ret = ExecuteHash(card, object);
		if(false == ret)
		{
			wxASSERT(false);
		}
	}

	virtual void Unexecute(const DataObject&)
	{
		wxASSERT(false);
	}

	virtual bool CanUndo() const
	{
		// Playing a card of any kind is never undoable.
		return false;
	}
};

IMPLEMENT_RULE(RulePlayCard, STANDARD);
