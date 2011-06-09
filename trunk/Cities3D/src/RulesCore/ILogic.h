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
class ILogic;
typedef boost::shared_ptr<ILogic> LogicPtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: ILogic
// 
// The interface of a single piece of game logic.  A piece
// of <ILogic> is not used on its own, but is incorporated 
// into an <IRule> that requires logic to determine what
// course of action to take.  The <ILogic> class provides a
// generic means of implementing such a thing.  It has one
// function, <Decide>, which takes an input <DataObject> and
// fills in an output <DataObject> with its rendered decision.
//
// All <ILogic> is originally created and stored in a <RuleSet> within
// a plugin object.  An <ILogic> within a <RuleSet> is stored in a string-based
// hash map.  All <ILogic> has a string that is the key used to execute it.
// When that <RuleSet> is loaded into the game via the
// <IPluginEngine>, all <ILogic> (and <IRules>) within the <RuleSet> 
// are transferred from their <RuleSet> hash map into the into the 
// main <RuleEngine> hash map, which, in the end, holds the aggregation of all
// <ILogic> in all <RuleSets> for the game.
//
// *NOTE*: An <ILogic>-derived class should be created and stored in a 
// <RuleSet> through one of the <IMPLEMENT_LOGIC>, <IMPLEMENT_LOGIC_MIXIN>, or 
// <IMPLEMENT_DEFAULT_BRANCH> macros in <RuleSetDatabase>.
//
// Project:
//     <RulesCore>
//
// Include:
//     ILogic.h
//
class RULES_EXPORT_SPEC ILogic
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Decide
	//
	// Decides based on logic.  Every piece of ILogic decides output
	// based on the given input.
	//
	// Parameters:
	//     input - The <DataObject> with data that helps the ILogic to
	//             make its decision.
	//     output - The resulting <DataObject> containing the output of the
	//              ILogic.  It comes in as an empty object and must be
	//              created and filled by the ILogic.
	//
	virtual void Decide(const DataObject &input, DataObject &output)=0;

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~ILogic
	// 
	// The ILogic destructor.  It is pure virtual because
	// this class must be derived from in order to be used.
	// 
	virtual ~ILogic()=0;
};

//---------------------------- DEFINES          -----------------------------//

//---------------------------- PROTOTYPES       -----------------------------//
inline ILogic::~ILogic() {}
