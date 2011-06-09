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

//---------------------------- USER INCLUDES    -----------------------------//

//---------------------------- DEFINES          -----------------------------//
#include "RulesExport.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: BaseMixin
// 
// The base class for any <IRule> or <ILogic> mixin objects.
// There are certain classes of rules or logic that require
// some kind of plugin architecture so that they can be extended
// by other modules.  For example, at the start of the game,
// one <IRule> may handle initializing all of the <Game> data.
// However, what if other plugin modules have new data that needs
// to be initialized?  Unless there was some way to notify that
// one <IRule> that there were other things that needed to be
// initialized, the situation would be hopeless.
//
// Enter BaseMixin.  The BaseMixin class provides a base class
// for new classes of mixin interfaces.  An <IRule> may be derived
// from a mixin interface so that it supports being extended.
// The BaseMixin class stores a string-based hash map internally.
// In the <RuleEngine>, when an <IRule> is executed, the <RuleEngine>
// checks to see if the <IRule> has BaseMixin in its inheritancy chart
// (via a dynamic_cast<>).  If so, the <RuleEngine> looks up in
// *its* internal string-based hash map to see if there are any mixin
// names for the <IRule>.  If so, it copies those hash pairs into
// the BaseMixin hash map.  A mixin value is nothing more than the
// name of another <IRule> or piece of <ILogic> that somehow extends
// the <IRule> with the mixin interface.  It is up to the particular
// derived mixin interface to determine what it does with the actual
// mixin names.  BaseMixin provides two means of accessing the hash
// map.  If the derived mixin interface just wants to iterate through
// all of the available mixins, it can use the <hash_begin> and 
// <hash_end> functions to iterate.  If it needs to lookup the value at
// a particular key, it can use the <hash_value> function. See 
// <LogicBooleanMixin> or <RuleExtensionMixin> for examples of derived
// mixin interfaces.
//
// Project:
//     <RulesCore>
//
// Include:
//     BaseMixin.h
//
class RULES_EXPORT_SPEC BaseMixin
{
	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor ~BaseMixin
	//
	// The BaseMixin destructor.  It is pure virtual
	// because this class must be derived from to be used.
	//
	virtual ~BaseMixin()=0;

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: hash_value
	//
	// Returns the mixin name at the given key.  If none exists,
	// an empty string is returned.  It is up to the calling function
	// to decide what to do with it.
	//
	// Parameters:
	//     key - The key to look up in the mixin hash map.
	//
	// Returns:
	//     A const reference to a string containing the mixin name at the
	//     given key.
	//
	const HashString &hash_value(const HashString &key) 
	{
		return mStringHash[key];
	}

	//-----------------------------------------------------------------------//
	// Function: hash_begin
	//
	// Returns an iterator to the start of the mixin hash map.
	//
	// Returns:
	//     A const iterator to the start of the mixin hash map.
	//
	IRuleEngine::StringHash::const_iterator hash_begin() const 
	{
		return mStringHash.begin();
	}

	//-----------------------------------------------------------------------//
	// Function: hash_end
	//
	// Returns an iterator to the end of the mixin hash map.
	//
	// Returns:
	//     A const iterator to the end of the mixin hash map.
	//
	IRuleEngine::StringHash::const_iterator hash_end() const 
	{
		return mStringHash.end();
	}

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Hash: mStringHash
	//
	// The string-based hash of mixin names.
	//
	IRuleEngine::StringHash mStringHash;

	//allows no write access
	friend class RuleEngine;
};

//---------------------------- PROTOTYPES       -----------------------------//
inline BaseMixin::~BaseMixin() {}
