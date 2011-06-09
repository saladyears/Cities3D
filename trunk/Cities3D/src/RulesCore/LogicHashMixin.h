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
// Class: LogicHashMixin
// 
// A mixin object for <IRules> or <ILogic> that allows specialized execution
// based on a hash key.  Some rules need the capability to be flexible
// for situations they have no knowledge of.  For instance, when creating
// text for the message window, the actual text may vary based on the <Game>
// mode.  By calling the <DecideHash> function with the name of the current 
// Mode, a mixed-in <ILogic> that handles that Mode, can be called that creates 
// the correct text for the View.
//
// Derived From:
//     <BaseMixin>
//
// Project:
//     <RulesCore>
//
// Include:
//     LogicHashMixin.h
//
class RULES_EXPORT_SPEC LogicHashMixin : public BaseMixin
{
	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: DecideHash
	//
	// Executes the <ILogic> whose name is stored in the hash
	// at the given key.  If no <ILogic> name exists at the given
	// hash key, the function immediately returns false and no
	// rule is executed.  It is up to the calling <IRule> or <ILogic>
	// to determine what to do in that case.
	//
	// Parameters:
	//     key - The key of the hashed <ILogic> to execute.
	//     input - The input <DataObject> to pass to the <ILogic>.  Must
	//             not be empty.
	//     output - The output <DataObject> to pass to the <ILogic>.  Must
	//              be empty.
	//
	// Returns:
	//     A bool giving whether an <ILogic> name existed at the
	//     given key.  The value is true if a name existed, false if
	//     not.
	//
	bool DecideHash(const HashString &key, const DataObject &input, 
		DataObject &output);
};

//---------------------------- PROTOTYPES       -----------------------------//

