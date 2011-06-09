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
#include "ModelExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class IGameObject;
typedef boost::shared_ptr<IGameObject> GameObjectPtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: IGameObjectEngine
//
// The interface of the <IGameObject> engine.  The <IGameObjectEngine>
// maintans a database of <IGameObjects> that have been provided by
// the loaded <RuleSets>.  The purpose of keeping track of all of the
// <IGameObjects> is related entirely to serialization of the <Game>
// and <PlayerGame> data.  Since <IGameObjects> are entirely pluggable,
// it would be impossible to know which objects to create and load when
// reading in data from the disk unless there was some method of creating
// the right object at the right time.  The IGameObjectEngine provides
// a way by storing <IGameObjects> with an associated key.  When an
// <IGameObject> needs to be created from the disk, the serialization function
// can call the <CreateGameObject> function to request the right 
// <IGameObject> for the key that it requires.
//
// Project:
//     <ModelCore>
//
// Include:
//     IGameObjectEngine.h
//
class MODEL_EXPORT_SPEC IGameObjectEngine
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: CreateGameObject
	//
	// Creates a new <IGameObject> that matches the given
	// name.
	//
	// Parameters:
	//     name - The name of the <IGameObject> to create.
	//
	// Returns:
	//     A newly cloned <IGameObject>.  May be NULL if no 
	//     object was found to match the name.
	//
	virtual IGameObject *CreateGameObject(const HashString &name) const=0;

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~IGameObjectEngine
	// 
	// The IGameObjectEngine destructor.  It is pure virtual because
	// this class must be derived from in order to be used.
	// 
	virtual ~IGameObjectEngine()=0;
};

//---------------------------- PROTOTYPES       -----------------------------//
inline IGameObjectEngine::~IGameObjectEngine() {}
