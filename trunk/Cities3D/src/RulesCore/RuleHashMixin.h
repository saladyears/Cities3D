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
#include "BaseMixin.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleHashMixin
// 
// A mixin object for <IRules> that allows specialized execution
// based on a hash key.  Some rules need the capability to be flexible
// for situations they have no knowledge of.  For instance, when loading
// a saved game from disk, the <IRule> has knowledge of the game's internal
// State field, but no knowledge of how to actually return to that state
// in the View because it is plugin-specific knowledge.  By calling the
// <ExecuteHash> function with the name of the current State, a mixed-in
// <IRule> that handles that State, provided by the plugin that sets the
// State, can be called that returns the View to its needed on-screen
// representation, allowing the game to continue from exactly where it
// left off.
//
// Derived From:
//     <BaseMixin>
//
// Project:
//     <RulesCore>
//
// Include:
//     RuleHashMixin.h
//
class RULES_EXPORT_SPEC RuleHashMixin : public BaseMixin
{
	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: ExecuteHash
	//
	// Executes the <IRule> whose name is stored in the hash
	// at the given key.  If no <IRule> name exists at the given
	// hash key, the function immediately returns false and no
	// rule is executed.  It is up to the calling <IRule> to determine
	// what to do in that case.
	//
	// Parameters:
	//     key - The key of the hashed <IRule> to execute.
	//     object - The <DataObject> to pass to the <IRule>.
	//
	// Returns:
	//     A bool giving whether an <IRule> name existed at the
	//     given key.  The value is true if a name existed, false if
	//     not.
	//
	bool ExecuteHash(const HashString &key, const DataObject &object);
};

//---------------------------- PROTOTYPES       -----------------------------//

