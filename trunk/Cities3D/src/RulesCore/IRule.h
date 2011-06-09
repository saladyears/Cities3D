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
#include "RulesExport.h"
#include "DataObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
class IRule;
typedef boost::shared_ptr<IRule> RulePtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: IRule
// 
// The interface for a single game rule.  The class <IRule> is an 
// implementation of the Command design pattern.  An <IRule> is a piece
// of code that *does* something in the <Game>.  What that something
// is is completely up to the <IRule>-derived object.  An <IRule>
// may call other <IRules> from within itself, or execute <ILogic> to
// determine what course of action to take.  <IRules> can also be
// extended by something derived from <BaseMixin> so that their behavior
// is pluggable, and different <RuleSets> can extend the functionality
// of exisiting <IRules>.  An <IRule> may make itself undoable by
// implementing the <IUndoable> interface.
//
// All <IRules> are originally created and stored in a <RuleSet> within
// a plugin object.  An <IRule> within a <RuleSet> is stored in a string-based
// hash map.  All <IRules> have a string that is the key used to execute them.
// When that <RuleSet> is loaded into the game via the
// <IPluginEngine>, all <IRules> (and <ILogic>) within the <RuleSet> 
// are transferred from their <RuleSet> hash map into the into the 
// main <RuleEngine> hash map, which, in the end, holds the aggregation of all
// <IRules> in all <RuleSets> for the game.
//
// *NOTE*: An <IRule>-derived class should be created and stored in a <RuleSet> 
// through one of the <IMPLEMENT_RULE>, <IMPLEMENT_RULE_REPLACE>, or 
// <IMPLEMENT_RULE_MIXIN> macros in <RuleSetDatabase>.
//
// Project:
//     <RulesCore>
//
// Include:
//     IRule.h
//
class RULES_EXPORT_SPEC IRule
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Execute
	//
	// Executes a rule.  May use the given <DataObject> to
	// assists its execution.
	//
	// Parameters:
	//     object - The <DataObject> that gives direction to the IRule.
	//
	virtual void Execute(const DataObject &object)=0;

	//-----------------------------------------------------------------------//
	// Function: Unexecute
	//
	// Unexecutes a rule.  Some rules can be undone, and must
	// do the exact opposite of their <Execute> function when called
	// with Unexecute.  However, not all rules need to be undone,
	// so the function is not pure virtual, but instead provides a
	// default (empty) function body.
	//
	// Parameters:
	//     object - The <DataObject> that gives direction to the IRule.
	//              Must the the same <DataObject> that Execute was originally
	//              called with.
	//
	virtual void Unexecute(const DataObject &) {}

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~IRule
	// 
	// The IRule destructor.  It is pure virtual because
	// this class must be derived from in order to be used.
	// 
	virtual ~IRule()=0;
};

//---------------------------- DEFINES          -----------------------------//

//---------------------------- PROTOTYPES       -----------------------------//
inline IRule::~IRule() {}
