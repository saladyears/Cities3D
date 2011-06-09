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
#include <boost/tuple/tuple.hpp>

//---------------------------- USER INCLUDES    -----------------------------//
#include "SelectionObject.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: SideSelectionObject
// 
// A <SelectionObject> for selecting sides of hexes.  Some objects
// reside on the sides of two hexes, like roads and ships.  During the
// game, there are opportunities for players to place a new one of these
// on the board.  The class SideSelectionObject provides a way for the
// View to represent the available sides for selection in the game.
// It stores a boost::tuple (see 
// <http://www.boost.org/libs/tuple/doc/tuple_users_guide.html>)
// of a side ID, <Vector> coordinates of the side, and a float rotation
// that should be applied to the selection to make it line up along the
// side of a hex.  The class <DrawSideSelectionObject> (located in <CoreRules>)
// is responsible for actually drawing the locations on the screen, but
// the SideSelectionObject provides all of the data for drawing, just
// like any other <IGameObject>/<IDrawObject> pairing.
//
// An <IRule> may create a SideSelectionObject and then add new side tuples
// to it via the <add> function.  The side tuples can be read out
// of the SideSelectionObject via the iterator-providing <selection_begin>
// and <selection_end> functions.
//
// Derived From:
//     <SelectionObject>
//
// Project:
//     <RulesCore>
//
// Include:
//     SideSelectionObject.h
//

class RULES_EXPORT_SPEC SideSelectionObject : public SelectionObject
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: SideSelectionObject 
	//
	// The SideSelectionObject constructor.  Stores the
	// given rule name and selection color.  See <SelectionObject>
	// for details.
	//
	SideSelectionObject(const HashString &rule, const ColorType color) 
		: SelectionObject(rule, color) {}
	
	//corners should have a selection ID and a location to be selected
	typedef boost::tuple<wxInt32, Vector, float> SideSelectionTuple;
	typedef std::list<SideSelectionTuple> SideSelectionList;

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: add
	//
	// Adds another side ID/<Vector>/rotation tuple to the std::list.
	//
	void add(const SideSelectionTuple &tuple) 
	{
		mSelectionList.push_back(tuple);
	}

	//-----------------------------------------------------------------------//
	// Function: selection_begin
	//
	// Returns an iterator to the start of the side ID/<Vector>/rotation 
	// std::list.
	//
	// Returns:
	//     A const_iterator to the start of the side ID/<Vector>/rotation 
	//     std::list.
	//
	SideSelectionList::const_iterator selection_begin();

	//-----------------------------------------------------------------------//
	// Function: selection_end
	//
	// Returns an iterator to the end of the side ID/<Vector>/rotation 
	// std::list.
	//
	// Returns:
	//     A const_iterator to the end of the side ID/<Vector>/rotation 
	//     std::list.
	//
	SideSelectionList::const_iterator selection_end();

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Clone
	//
	// Clones the SideSelectionObject.  *Unused*.
	//
	// Returns:
	//     A NULL pointer.
	//
	virtual IGameObject *Clone() 
	{
		return NULL;
	}

	//-----------------------------------------------------------------------//
	// Function: name
	//
	// Resets the class name of the SideSelectionObject.
	//
	// Returns:
	//     A const reference to a string containing the
	//     class name of the SideSelectionObject.
	//
	virtual const HashString& name() 
	{
		return shSideSelectionObject;
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
	// The std::list of corner ID/<Vector>/rotation tuples that
	// are selectable sides.
	//
	SideSelectionList mSelectionList;
};

//---------------------------- PROTOTYPES       -----------------------------//
