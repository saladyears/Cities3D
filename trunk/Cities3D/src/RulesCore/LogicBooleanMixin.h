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
#include "Logic.h"
#include "GameEngine.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicBooleanMixin
// 
// A mixin object for <ILogic> that makes true or false decisions.
// There are certain types of logic decisions the game that
// often require a set of criteria to be fulfilled, in order for
// that logic to be evaluated as true (or false).  However, there
// is always the possibility that a plugin module might have a
// reason to further restrict or expand on a particular logic
// decision.  The LogicBooleanMixin class allows this to take place.
//
// The class LogicBooleanMixin is a <BaseMixin>-derived template class 
// that takes a bool value as its one template parameter.  When the
// LogicBooleanMixin class is called upon to make a decision (via its
// <Decide> function), it iterates through all of the mixin string names
// stored in <BaseMixin>'s hash map.  Each of these string names must
// be the name of a piece of <ILogic> in the <RuleEngine>.  The 
// LogicBooleanMixin class goes through the mixed-in <ILogic>, calling
// each via the <RuleEngine>.  As soon as one of them returns a value
// that is the opposite of the boolean template parameter, the 
// LogicBooleanMixin class immediately exits with that value.  If all
// mixed-in <ILogic> passes, the boolean template value is returned.
//
// This is all rather abstract, so here's an example.  Let's say it's 
// the beginning of the game and players are placing initial settlements
// down.  Well, there are some well-known rules for where settlements
// can and can't go (like not within one corner of another settlement, not
// ON another settlement, etc.).  When the game needs to draw available spots 
// for settlements on the board, it needs some way of removing every spot that
// is illegal to place on.  The perfect situation for LogicBooleanMixin!  
// The game can just assume that every hex corner is a valid location for a 
// settlement, unless proven otherwise.  It is up to the LogicBooleanMixin to 
// prove otherwise. So, in actuality, the <LogicPlaceInitialSettlement> class 
// inherits from LogicBooleanMixin with a the template bool parameter set to 
// true. When the <Decide> function is called for the class, the 
// LogicBooleanMixin kicks into action.  It iterates through each of the 
// mixed-in hex-corner-deciding <ILogic>.  Each mixed-in <ILogic> examines the 
// hex corner it is given and, if it violates that logic, return false, in 
// which case the LogicBooleanMixin returns false for that hex corner, and it 
// cannot be built on.
//
// Derived From:
//     <Logic>
//     <BaseMixin>
//
// Project:
//     <RulesCore>
//
// Include:
//     LogicBooleanMixin.h
//

//this mixin class allows a piece of logic to have boolean logic that 
//starts off with one value unless the logic the mixin represents 
//determine that the initial value is invalid
template <bool INITIAL = true>
class LogicBooleanMixin : public Logic, public BaseMixin
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Decide
	//
	// Renders the boolean decision.  Calls the <Execute>
	// function to iterate through all of the stored mixin
	// <ILogic>.
	//
	// Parameters:
	//     input - The input <DataObject>.  Must not be empty.
	//     output - The output <DataObject>.  Must be empty.  Will
	//              be filled with a bool value on the way out that
	//              is the rendered decision of the LogicBooleanMixin.
	//
	virtual void Decide(const DataObject &input, DataObject &output);

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Evaluate
	//
	// Returns the rendered decision of the mixed-in <ILogic>.
	// Iterates through each of the mixed-in <ILogic> names
	// in the <BaseMixin> hash map.  If a piece of <ILogic> returns
	// with a value that invalidates the bool template parameter,
	// Evaluate immediately returns with the invalid bool.  If all
	// mixed-in <ILogic> returns with the same bool value, the
	// original template parameter was valid and it is returned
	// as the rendered decision.
	//
	// Parameters:
	//     input - The input <DataObject>.
	//
	// Returns:
	//     bool - A bool giving the rendered decision of all mixed-in
	//            <ILogic>.
	//
	bool Evaluate(const DataObject &input);
};

//---------------------------- PROTOTYPES       -----------------------------//
template <bool INITIAL>
void LogicBooleanMixin<INITIAL>::Decide(const DataObject &input, 
										DataObject &output)
{
	output = DataObject(Evaluate(input));
}

template <bool INITIAL>
bool LogicBooleanMixin<INITIAL>::Evaluate(const DataObject &input)
{
	//set the initial value
	bool initial = INITIAL;

	//test each piece of the logic to see if it invalidates
	//the initial setting
	IRuleEngine::StringHash::const_iterator it, itEnd(hash_end());
	for(it = hash_begin(); it != itEnd; ++it)
	{
		DataObject output;

		//apply the logic
		RULE.Decide(it->second, input, output);
		
		//if this decision invalidates the initial value,
		//we're done
		bool evaluated = output.read<bool>();

		if(evaluated != initial)
		{
			return evaluated;
		}
	}

	//we made it through and the initial value is valid
	return initial;
}
