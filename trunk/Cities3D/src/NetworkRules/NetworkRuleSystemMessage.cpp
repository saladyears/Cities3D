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
#include "HTML.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkRuleSystemMessage
// 
// Formats a text message for display in an HTML chat window and sends it on.
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
// Transmits To:
//     eventSystemMessage
//
class NetworkRuleSystemMessage 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxString str = object.read<wxString>();
		const wxDateTime &time = object.read<wxDateTime>(1);

		wxString timestamp = swBracketLeft + time.FormatISOTime() + swBracketRightSpace;
		HTML::Font(timestamp, shSystemMessageTimestamp, shSystemMessageTimestamp);

		// Add some formatting.
		HTML::Font(
			HTML::Italics(
				HTML::Bold(str)));

		str = timestamp + str;
		HTML::Break(str);

		Controller::get().Transmit(shEventSystemMessage, 
			DataObject(str));
	}

	virtual bool CanUndo() const
	{
		// Since system messages count as a new rule in network games, they
		// would normally turn off any rule that could be undone, so just
		// return whether undo is currently available.
		return RULE.PotentialUndo() || RULE.CanUndo();
	}

	virtual void Unexecute(const DataObject &)
	{
		// If it's possible that we can undo the system message, just let the
		// undo continue to the next rule.
		RULE.Undo();
	}
};

IMPLEMENT_RULE(NetworkRuleSystemMessage, NETWORK)
