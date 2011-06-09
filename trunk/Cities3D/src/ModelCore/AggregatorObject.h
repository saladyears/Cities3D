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

//---------------------------- TYPEDEFS         -----------------------------//
class IGameObject;
typedef boost::shared_ptr<IGameObject> GameObjectPtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: AggregatorObject
// 
// Aggregates <IGameObjects> so they can be sent to the renderer in one batch.
// At various points in the game, many new game objects are created at once,
// such as the board being constructed, or seaports being turned on and off.
// Using an AggregatorObject to pass them all to the view at once allows for
// much greater function call efficiency then calling the view dozens of times
// with single objects.
//
// Project:
//     <ModelCore>
//
// Include:
//     AggregatorObject.h
//
class AggregatorObject
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Typedefs
	//

	//-----------------------------------------------------------------------//
	// Typedef: GameObjectList
	//
	// A std::list container of IGameObject shared_ptrs.
	//
	typedef std::list<GameObjectPtr> GameObjectList;

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: objects_begin
	//
	// Provides an iterator to the start of the IGameObject list.
	//
	// Returns:
	//     A GameObjectList const_iterator to the start of the list.
	//
	GameObjectList::const_iterator objects_begin() {return mObjectList.begin();}

	//-----------------------------------------------------------------------//
	// Function: objects_end
	//
	// Provides an iterator to the end of the IGameObject list.
	//
	// Returns:
	//     A GameObjectList const_iterator to the end of the list.
	//
	GameObjectList::const_iterator objects_end() {return mObjectList.end();}

	//-----------------------------------------------------------------------//
	// Function: add
	//
	// Adds a new <IGameObject> shared_ptr to the end of the list.
	//
	// Parameters:
	//     object - The IGameObject shared_ptr to add.
	//
	void add(const GameObjectPtr &object) {mObjectList.push_back(object);}

	//-----------------------------------------------------------------------//
	// Function: list
	//
	// Returns the raw std::list of <IGameObjects>.
	//
	// Returns:
	//     A reference to the std::list of <IGameObjects>.
	//
	GameObjectList &list() {return mObjectList;}

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Array: mObjectList
	//
	// The std::list of IGameObject shared_ptrs.
	//
	GameObjectList mObjectList;
};

typedef boost::shared_ptr<AggregatorObject> AggregatorObjectPtr;

//---------------------------- PROTOTYPES       -----------------------------//
