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
#include "SelectionObject.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: CornerSelectionObject
// 
// A <SelectionObject> for selecting corners of hexes.  Some objects
// reside on the corners of three hexes, like settlements, cities,
// knights, etc.  During the game, there are opportunities for players
// to place a new one of these objects on the board.  The class
// CornerSelectionObject provides a way for the View to represent the
// available corners for selection in the game.  It stores a list of
// std::pair<> objects of a corner ID and the <Vector> coordinates of 
// the corner. The class <DrawCornerSelectionObject> (located in <CoreRules>) 
// is responsible for actually drawing the locations on the screen, but
// CornerSelectionObject provides all of the data for drawing, just
// like any other <IGameObject>/<IDrawObject> pairing.
//
// An <IRule> may create a CornerSelectionObject and then add new corner
// pairs to it via the <add> function.  The corner pairs can be read out
// of the CornerSelectionObject via the iterator-providing <selection_begin>
// and <selection_end> functions.
//
// Derived From:
//     <SelectionObject>
//
// Project:
//     <RulesCore>
//
// Include:
//     CornerSelectionObject.h
//

class RULES_EXPORT_SPEC CornerSelectionObject : public SelectionObject
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: CornerSelectionObject 
	//
	// The CornerSelectionObject constructor.  Stores the
	// given rule name and selection color.  See <SelectionObject>
	// for details.
	//
	CornerSelectionObject(const HashString &rule, const ColorType color) 
		: SelectionObject(rule, color) {}
	
	//corners should have a selection ID and a location to be selected
	typedef std::pair<wxInt32, Vector> CornerSelectionPair;
	typedef std::list<CornerSelectionPair> CornerSelectionList;

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: add
	//
	// Adds another corner ID/<Vector> pair to the std::list.
	//
	void add(const CornerSelectionPair &pair) {mSelectionList.push_back(pair);}

	//-----------------------------------------------------------------------//
	// Function: selection_begin
	//
	// Returns an iterator to the start of the corner ID/<Vector> std::list.
	//
	// Returns:
	//     A const_iterator to the start of the corner ID/<Vector> std::list.
	//
	CornerSelectionList::const_iterator selection_begin();

	//-----------------------------------------------------------------------//
	// Function: selection_end
	//
	// Returns an iterator to the end of the corner ID/<Vector> std::list.
	//
	// Returns:
	//     A const_iterator to the end of the corner ID/<Vector> std::list.
	//
	CornerSelectionList::const_iterator selection_end();

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Clone
	//
	// Clones the CornerSelectionObject.  *Unused*.
	//
	// Returns:
	//     A NULL pointer.
	//
	virtual IGameObject *Clone() {return NULL;}

	//-----------------------------------------------------------------------//
	// Function: name
	//
	// Resets the class name of the CornerSelectionObject.
	//
	// Returns:
	//     A const reference to a string containing the
	//     class name of the CornerSelectionObject.
	//
	virtual const HashString& name() 
	{
		return shCornerSelectionObject;
	}

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Object: mSelectionList
	//
	// The std::list of corner ID/<Vector> pairs that
	// are selectable corners.
	//
	CornerSelectionList mSelectionList;
};

//---------------------------- PROTOTYPES       -----------------------------//
