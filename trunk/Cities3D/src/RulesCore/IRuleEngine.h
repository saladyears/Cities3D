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
#include <list>
#include <wx/hashmap.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "RulesExport.h"

//---------------------------- TYPEDEFS         -----------------------------//
class IRule;
typedef boost::shared_ptr<IRule> RulePtr;

class ILogic;
typedef boost::shared_ptr<ILogic> LogicPtr;

class DataObject;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: IRuleEngine
// 
// The interface for the rule engine.  The IRuleEngine manages the
// database of <IRule> and <ILogic> and provides functions for
// executing <IRules> or <ILogic>.  All of the data is stored in
// string-based hash maps so that an <IRule> or piece of <ILogic>
// is executed just by calling <Execute> or <Decide> with the name 
// of the <IRule> or <ILogic>.  IRuleEngine also provides functions
// for dealing with <IRules> that can be undone.  If the last
// rule executed is undoable, the <CanUndo> function returns true.
// To actually undo the last action, the <Undo> function is
// provided. The IRuleEngine is accessible through the <GameEngine>.
//
// Project:
//     <RulesCore>
//
// Include:
//     IRuleEngine.h
//
class RULES_EXPORT_SPEC IRuleEngine
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
	// Executes the given rule with the given <DataObject>.
	//
	// Parameters:
	//     rule - The name of the <IRule> to execute.
	//     object - The <DataObject> to pass to the <IRule>.
	//
	virtual void Execute(const HashString &rule, 
		const DataObject &object) const=0;

	//-----------------------------------------------------------------------//
	// Function: AsynchExecute
	//
	// Executes the given rule asynchronously with the given <DataObject>.
	//
	// Parameters:
	//     rule - The name of the <IRule> to execute.
	//     object - The <DataObject> to pass to the <IRule>.
	//
	virtual void AsynchExecute(const HashString &rule, 
		const DataObject &object) const=0;

	//-----------------------------------------------------------------------//
	// Function: Decide
	//
	// Calls the given logic with the given <DataObjects>.
	//
	// Parameters:
	//     logic - The name of the <ILogic> to call.
	//     input - The input <DataObject>.
	//     output - The output <DataObject>.  Must be NULL.
	//
	virtual void Decide(const HashString &logic, const DataObject &input, 
		DataObject &output) const=0;

	//-----------------------------------------------------------------------//
	// Function: CanUndo
	//
	// Returns whether there is anything on the undo stack
	// that can be undone.
	//
	// Returns:
	//     A bool giving whether the last action can be undone.
	//     The value is true if the action is undoable, false if not.
	//
	virtual bool CanUndo() const=0;

	//-----------------------------------------------------------------------//
	// Function: InUndo
	//
	// Returns whether a rule is currently being undone.
	//
	// Returns:
	//     A bool that is true if a rule is being undone and false if not.
	//
	virtual bool InUndo() const=0;

	//-----------------------------------------------------------------------//
	// Function: PotentialUndo
	//
	// Returns whether the current rule stack has the potential of being 
	// undone.
	//
	// Returns:
	//     A bool that is true if the current rule stack has the potential to
	//     be undone and false if not.
	//
	virtual bool PotentialUndo() const=0;

	//-----------------------------------------------------------------------//
	// Function: Undo
	//
	// Undoes the last action.
	//
	virtual void Undo()=0;

	//-----------------------------------------------------------------------//
	// Function: SetWinner
	//
	// Sets the winner of the game.
	//
	// Parameters:
	//     player - The index of the winning player.
	//
	virtual void SetWinner(wxInt32 player)=0;

	//-----------------------------------------------------------------------//
	// Function: GetWinner
	//
	// Gets the winner of the game.
	//
	// Returns:
	//     The winner of the game, or -1 if no one has won yet.
	//
	virtual wxInt32 GetWinner() const=0;

	WX_DECLARE_HASH_STRING_HASH_MAP(RulePtr, RuleHash);
	WX_DECLARE_HASH_STRING_HASH_MAP(LogicPtr, LogicHash);
	WX_DECLARE_HASH_STRING_HASH_MAP(HashString, StringHash);
	WX_DECLARE_HASH_STRING_HASH_MAP(StringHash, MixinHash);

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~IRuleEngine
	// 
	// The IRuleEngine destructor.  It is pure virtual because
	// this class must be derived from in order to be used.
	// 
	virtual ~IRuleEngine()=0;
};

//---------------------------- DEFINES          -----------------------------//

//---------------------------- PROTOTYPES       -----------------------------//
inline IRuleEngine::~IRuleEngine() {}
