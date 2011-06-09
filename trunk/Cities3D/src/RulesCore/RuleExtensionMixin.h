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

//---------------------------- DEFINES          -----------------------------//
#include "BaseMixin.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleExtensionMixin
// 
// A mixin object for <IRules> that extends the <IRule> with other
// <IRules>.  Some rules, in particular, rules related to the
// startup and initialization of the <Game> data and the board,
// want to allow plugin modules to be able to add their own
// plugin-specific extensions to the <IRule>, without having to have
// any knowledge of, or the ability to keep track of, any other plugin
// modules that want to do the same.  The RuleExtensionMixin meets this
// need.
//
// If an <IRule> is derived from RuleExtensionMixin, it can choose
// to execute all available extension rules by calling the 
// <ExecuteExtensions> function.  The <ExecuteExtensions> function
// iterates through all of the mixed-in rule names stored in the 
// <BaseMixin> hash map, calling each of them via the <RuleEngine>.
// This allows plugin modules to provide their own plugin-specific
// extensions to <IRules> that need it.
//
// Derived From:
//     <BaseMixin>
//
// Project:
//     <RulesCore>
//
// Include:
//     RuleExtensionMixin.h
//
class RULES_EXPORT_SPEC RuleExtensionMixin : public BaseMixin
{
	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: ExecuteExtensions
	//
	// Executes all of the mixed-in <IRule> extensions.
	// Iterates through all of the mixin names stored in
	// the <BaseMixin> hash map and calls the <RuleEngine>
	// to execute each of them.
	//
	// Parameters:
	//     object - The <DataObject> to pass to each mixed-in <IRule>.
	//
	void ExecuteExtensions(const DataObject &object);
};

//---------------------------- PROTOTYPES       -----------------------------//
