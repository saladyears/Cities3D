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
#pragma once

#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//
#include "DataObject.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxRuleEvent
//
// Encapsulates a <Rule> and <DataObject> for passing from one thread to 
// another.
//
// Derived From:
//     <wxNotifyEvent>
//
// Project:
//     <RulesCore>
//
// Include:
//     RuleEvent.h
//
class wxRuleEvent : public wxNotifyEvent
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxRuleEvent
	//
	// The wxRuleEvent constructor.
	//
	// Parameters:
	//     rule - The <Rule> to execute.
	//     data - The <DataObject> associated with the <Rule>.
	//     asynch - Whether to execute the rule asynchronously or not.
	//              Executing a rule asynchronously prevents it from being sent
	//              to the network.
	//
	wxRuleEvent(const HashString &rule, const DataObject &data, 
		const bool asynch = false);

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Clone
	//
	// Clones the event for passing it between threads.
	//
	// Returns:
	//     A cloned version of the event.
	//
	virtual wxEvent *Clone() const;

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Rule
	//
	// Returns the <Rule> associated with the event.
	//
	// Returns:
	//     A string that is the <Rule> associated with the event.
	//
	const HashString &Rule() const 
	{
		return mRule;
	}

	//-----------------------------------------------------------------------//
	// Function: Data
	//
	// Returns the <DataObject> associated with the event.
	//
	// Returns:
	//     The <DataObject> associated with the event.
	//
	const DataObject &Data() const 
	{
		return mData;
	}

	//-----------------------------------------------------------------------//
	// Function: Asynch
	//
	// Returns whether the <Rule> should be executed asynchronously or not.
	//
	// Returns:
	//     A bool that is true if the <Rule> should be executed asynchronously,
	//     and false if not.
	//
	const bool Asynch() const 
	{
		return mAsynch;
	}

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// String: mRule
	//
	// The rule to execute.
	//
	HashString mRule;

	//-----------------------------------------------------------------------//
	// Object: mData
	//
	// The <DataObject> associated with the rule.
	//
	DataObject mData;

	//-----------------------------------------------------------------------//
	// Object: mAsynch
	//
	// A flag that is true if the <Rule> should be executed asynchronously and
	// false if not.
	//
	bool mAsynch;
};

//---------------------------- PROTOTYPES       -----------------------------//

// Declare everything necessary to have wxEvtHandlers be able to accept these
// events.
DECLARE_LOCAL_EVENT_TYPE(wxEVT_RULE, 0)

typedef void (wxEvtHandler::*wxRuleEventFunction)(wxRuleEvent &);

#define EVT_RULE(id, fn) \
	DECLARE_EVENT_TABLE_ENTRY(wxEVT_RULE, id, -1, \
	(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) \
	(wxNotifyEventFunction) wxStaticCastEvent(wxRuleEventFunction, &fn), \
	(wxObject *) NULL),
