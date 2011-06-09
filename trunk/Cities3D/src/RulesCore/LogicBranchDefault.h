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
#include "Logic.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicBranchDefault
// 
// Provides a default branch in the rule chain.  The rule chain is stored
// in the <RuleEngine> using pieces of priority branching <ILogic>.
// A rule chain should be flexible, allowing plugin modules to alter it by 
// adding overriding branches, if needed.  However, if no plugin module 
// alters the rule chain at a specific juncture, there must be something 
// that gives that next <IRule> to execute in the chain.  Since this 
// circumstance is so common, the class LogicBranchDefault provides a simple 
// way of creating a default rule branch in the rule chain.  The 
// LogicBranchDefault is simply created with the name of the <IRule> to branch 
// to and can then be added to the <RuleSetDatabase> via the 
// <IMPLEMENT_DEFAULT_BRANCH> macro.
//
// Derived From:
//     <Logic>
//
// Project:
//     <RulesCore>
//
// Include:
//     LogicBranchDefault.h
//
class LogicBranchDefault : public Logic
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: LogicBranchDefault
	//
	// The LogicBranchDefault constructor.  Stores the
	// given <IRule> name.
	//
	// Parameters:
	//     rule - The name of the default <IRule> to
	//            branch to in the rule chain.  Must not be
	//            empty.
	//
	LogicBranchDefault(const HashString &rule) 
		: mRule(rule) 
	{
		wxASSERT(false == mRule.empty());
	}

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Decide
	//
	// Returns the default rule in the output <DataObject>.
	// Since the purpose of the class is to have a default 
	// <IRule> to branch to, the class simply returns the name
	// of the default <IRule> branch.
	// 
	// Parameters:
	//     input - The input <DataObject>.  *Unused*.
	//     output - The output <DataObject>.  Filled with the name
	//              of the default <IRule>.
	//
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(mRule);
	}

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: LogicBranchDefault
	//
	// The default LogicBranchDefault constructor.  It
	// is declared but not instantiated so that developers
	// will get a compile error if they forget to create a 
	// LogicBranchDefault without an <IRule> name.
	//
	LogicBranchDefault();

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// String: mRule
	//
	// The name of the default <IRule> to branch to
	// in the rule chain.
	//
	HashString mRule;
};

//---------------------------- PROTOTYPES       -----------------------------//

