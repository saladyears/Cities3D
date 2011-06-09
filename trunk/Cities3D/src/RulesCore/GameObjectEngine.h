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
#include "RulesExport.h"
#include "IGameObjectEngine.h"
#include "RuleSet.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: GameObjectEngine
// 
// The default implementation of the <IGameObjectEngine> interface.
// GameObjectEngine stores all of the <IGameObjects> provided by
// the loaded <RuleSets> internally in a hash map container.  The
// functions required by the <IGameObjectEngine> interface are implemented
// by taking the hash map and performing the necessary operations.
// See the function documentation below for more details.
//
// Derived From:
//     <IGameObjectEngine>
//
// Project:
//     <RulesCore>
//
// Include:
//     GameObjectEngine.h
//

class RULES_EXPORT_SPEC GameObjectEngine : public IGameObjectEngine
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Stop
	//
	// Stops the GameObjectEngine.  Clears the hash of 
	// <IGameObjects>.
	//
	void Stop();

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: CreateGameObject
	//
	// Creates a new <IGameObject> that matches the given
	// name.  Searches the hash for an entry at the given name.
	// If an object exists there, clones the object and returns
	// the clone.  Otherwise, returns an empty object.
	//
	// Parameters:
	//     name - The name of the <IGameObject> to create.
	//
	// Returns:
	//     A newly cloned <IGameObject>.  May be NULL if no 
	//     object was found to match the name.
	//
	virtual IGameObject *CreateGameObject(const HashString &name) const;

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Variable: mObjectHash
	//
	// The hash of <IGameObjects> available to the game.
	//
	RuleSet::GameObjectHash mObjectHash;
	
	friend class PluginEngine;
};

//---------------------------- PROTOTYPES       -----------------------------//
