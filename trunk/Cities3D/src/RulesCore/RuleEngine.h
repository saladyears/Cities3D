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
#include <map>
#include <list>
#include <stack>
#include <wx/hashmap.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "RulesExport.h"
#include "IRuleEngine.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class BaseMixin;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleEngine
// 
// The default implementation of the <IRuleEngine> interface.
// RuleEngine stores all of the <IRules>, <ILogics>, mixin
// strings, and priority branching logic provided by the
// loaded <RuleSets> in string-based hash maps.  The functions
// required by the <IRuleEngine> interface are implemented
// by taking the string-based hash maps and performing the
// necessary operations.  See the function and variable 
// documentation below for more details.
//
// Derived From:
//     <IRuleEngine>
//
// Project:
//     <RulesCore>
//
// Include:
//     RuleEngine.h
//

class RULES_EXPORT_SPEC RuleEngine : public IRuleEngine
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: RuleEngine
	//
	// The RuleEngine constructor.
	//
	RuleEngine();

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~RuleEngine()
	//
	// The RuleEngine destructor.
	//
	~RuleEngine();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Stop
	//
	// Stops the RuleEngine.  Cleans out the undo stack.  Clears
	// the rule hash, logic hash, mixin hash, and branching logic
	// hash.
	//
	void Stop();

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Execute
	//
	// If the network is active, sends the rule to the <NetworkEngine>, 
	// causing the rule to be executed asynchronously when the network decides
	// to execute it.  Otherwise, executes the rule directly by calling
	// <AsynchExecute>.
	//
	// Parameters:
	//     rule - The name of the <IRule> to execute.
	//     object - The <DataObject> to pass to the <IRule>.
	//
	virtual void Execute(const HashString &rule, const DataObject &object) const;

	//-----------------------------------------------------------------------//
	// Function: AsynchExecute
	//
	// Executes the given <IRule> with the given <DataObject>. Looks up the 
	// <IRule> name in the string hash map.  If the string name does not exist 
	// in the hash, an error is thrown to the screen.  If the <IRule> is 
	// potentially undoable, it may be added to the undo stack if its CanUndo 
	// function returns true.  If there are any mixins that belong to this 
	// <IRule>, they are added to the <IRule> before it executes. After the 
	// rule executes,  the <Branch> function is called to see if the <IRule> 
	// has any branching logic that needs to get called.
	//
	// Parameters:
	//     rule - The name of the <IRule> to execute.
	//     object - The <DataObject> to pass to the <IRule>.
	//
	virtual void AsynchExecute(const HashString &rule, 
		const DataObject &object) const;

	//-----------------------------------------------------------------------//
	// Function: Decide
	//
	// Calls the given logic with the given <DataObjects>.  Looks
	// up the <ILogic> name in the string hash map.  If the string
	// name does not exist in the hash, an error is thrown to the screen.
	// If there are any mixins that belong to this <ILogic>, they
	// are added to the <ILogic> before it is called.  The output
	// <DataObject> must be empty before the <ILogic's> Decide function
	// is called and non-empty afterwards.
	//
	// Parameters:
	//     logic - The name of the <ILogic> to call.
	//     input - The input <DataObject>.
	//     output - The output <DataObject>.  Must be NULL.
	//
	virtual void Decide(const HashString &logic, const DataObject &input, 
		DataObject &output) const;

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
	virtual bool CanUndo() const;

	//-----------------------------------------------------------------------//
	// Function: InUndo
	//
	// Returns whether a rule is currently being undone.
	//
	// Returns:
	//     A bool that is true if a rule is being undone and false if not.
	//
	virtual bool InUndo() const
	{
		return mInUndo;
	}

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
	virtual bool PotentialUndo() const;
	
	//-----------------------------------------------------------------------//
	// Function: Undo
	//
	// Undoes the last action.  Pops the last <IRule> name/<DataObject>
	// pair off the undo stack and calls that <IRules> Unexecute
	// function with the <DataObject>.  Plays the undo sound.
	//
	virtual void Undo();

	//-----------------------------------------------------------------------//
	// Function: SetWinner
	//
	// Sets the winner of the game.
	//
	// Parameters:
	//     player - The index of the winning player.
	//
	virtual void SetWinner(wxInt32 player)
	{
		mWinner = player;
	}
	
	//-----------------------------------------------------------------------//
	// Function: GetWinner
	//
	// Gets the winner of the game.
	//
	// Returns:
	//     The winner of the game, or -1 if no one has won yet.
	//
	virtual wxInt32 GetWinner() const
	{
		return mWinner;
	}

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Branch
	//
	// Branches down the rule chain.  Looks up the given <IRule>
	// in the hash of rule chain logic.  If an entry is found
	// with the given <IRule> name, the branch logic at that entry
	// is called in priority order.  If any of the branch logic
	// returns the name of another <IRule>, that <IRule> is then
	// immediately executed via the <Execute> function.  In this way
	// branching logic for <IRules> may be implemented without the
	// <IRules> themselves knowing about each other.
	//
	// Parameters:
	//     rule - The name of the <IRule> to branch from.
	//     object - The <DataObject> to pass down the rule chain.
	//
	void Branch(const HashString &rule, const DataObject &object) const;

	//-----------------------------------------------------------------------//
	// Function: AddMixins
	//
	// Adds any mixins to the given <IRule> or <ILogic>.  Looks
	// up the name in the mixin hash.  If there are any mixin names
	// available for that <IRule> or <ILogic> name, they are added
	// to the given <BaseMixin> pointer so that the <IRule> or
	// <ILogic> has access to them during its execution.
	//
	// Parameters:
	//     name - The name of the <IRule> or <ILogic> to look up
	//            mixins for.
	//     pMixin - A pointer to the <BaseMixin> object that will receive
	//              any mixin data.  It is safe for the pointer to be NULL.
	//
	void AddMixins(const HashString &name, BaseMixin *pMixin) const;

	//-----------------------------------------------------------------------//
	// Function: ClearUndoStack
	//
	// Clears out the undo stack.
	//
	void ClearUndoStack() const;

	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnServerConnection
	//
	// Fires up a <wxServerConnectionThread> after locking down all of the data
	// needed.
	//
	// Parameters:
	//     object - The <DataObject> containing the <wxCities3DSocket> and
	//              <Game> information.
	//
	void OnServerConnection(const DataObject &object);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Hash: mRuleHash
	//
	// The string-based hash of <IRule> objects.  The rule hash
	// contains the <IRules> specific to the game being played, copied
	// from all of the <RuleSets> loaded by the <IPluginEngine>.  Each
	// <IRule> is stored in a one to one mapping with a string that is the
	// rule name.  Rules are executed by calling the <Execute> function
	// with the rule name and a <DataObject> to pass to the <IRule>.
	//
	RuleHash mRuleHash;

	//-----------------------------------------------------------------------//
	// Hash: mLogicHash
	//
	// The string-based hash of <ILogic> objects.  The logic hash
	// contains the <ILogic> specific to the game being played, copied
	// from all of the <RuleSets> loaded by the <IPluginEngine>.  Each
	// piece of <ILogic> is stored in a one to one mapping with a string
	// that is the logic name.  Logic is executed by calling the <Decide>
	// function with the logic name, an input <DataObject>, which may not
	// be empty, and an output <DataObject>, which must be empty, and is
	// filled with the rendered decision of the piece of <ILogic>.
	//
	LogicHash mLogicHash;

	//-----------------------------------------------------------------------//
	// Hash: mMixinHash
	//
	// The string-based hash of string mixin names.  Each <IRule> or
	// <ILogic> that has mixins provided by the loaded <RuleSets> has
	// an entry by name in the mixin hash.  At each mixin hash entry
	// is stored another string-based hash map that contains the actual
	// mixin entry names for that <IRule> or <ILogic>.
	//
	mutable MixinHash mMixinHash;

	typedef std::map<wxInt32, LogicPtr, std::greater<wxInt32> > BranchPriorityMap;
	WX_DECLARE_HASH_STRING_HASH_MAP(BranchPriorityMap, BranchPriorityHash);

	//-----------------------------------------------------------------------//
	// Hash: mBranchHash
	//
	// The string-based hash of priority branching logic.  Some
	// <RuleSets> need certain <IRules> to immediately branch to
	// another <IRule> after it has finished executing.  However, other
	// <RuleSets> may need to overload even this existing branching.
	// The priority branch hash provides a mechanism for this to happen.
	// The name of each rule with branching logic is stored as a key 
	// in the hash.  For each key, a std::map of integer/<ILogic> pairs
	// is stored, with higher integer values being stored first (meaning,
	// they have greater priority).  When the <Branch> function is called,
	// the given rule name is looked up in the priority branching hash.  If
	// it is discovered, each of its associated <ILogic> is called in the
	// priority order.  If any piece of <ILogic> returns the name of a
	// rule, that <IRule> is immediately branched to with the <Execute>
	// function.
	//
	mutable BranchPriorityHash mBranchHash;

	//this insanity stores the rule and data object that can be undone,
	//as well as the rule and data object that immediately proceeded that
	//rule, so that when it IS undone, we know how to get things going again
	//this will probably be made better in the future (I hope!)
	typedef std::pair<HashString, DataObject> UndoPair;
	typedef std::list<UndoPair> UndoList;
	typedef std::stack<UndoList> UndoStack;

	//-----------------------------------------------------------------------//
	// Object: mWorkingUndoList
	//
	// The current working undo list.
	//
	mutable UndoList mWorkingUndoList;

	//-----------------------------------------------------------------------//
	// Object: mUndoStack
	//
	// The undo stack.  Still volatile.
	//
	mutable UndoStack mUndoStack;

	//-----------------------------------------------------------------------//
	// Int: mRuleDepth
	//
	// The depth of the rule call stack.
	//
	mutable wxInt32 mRuleDepth;

	//-----------------------------------------------------------------------//
	// Int: mUndoDepth
	//
	// The depth of the undo call stack.
	//
	wxInt32 mUndoDepth;

	//-----------------------------------------------------------------------//
	// Int: mWinner
	//
	// The index of the game-winning player.
	//
	mutable wxInt32 mWinner;

	//-----------------------------------------------------------------------//
	// Bool: mCanUndo
	//
	// A flag that is true if the current rule chain is undoable and false if
	// not.
	//
	mutable bool mCanUndo;

	//-----------------------------------------------------------------------//
	// Bool: mInUndo
	//
	// A flag that is true if the rule engine is currently undoing a rule and
	// false if not.
	//
	bool mInUndo;

	//-----------------------------------------------------------------------//
	// Object: mLock
	//
	// The recursive mutex that provides thread-safety for rule execution.
	//
	mutable wxMutex mLock;

	friend class PluginEngine;
};

//---------------------------- PROTOTYPES       -----------------------------//
