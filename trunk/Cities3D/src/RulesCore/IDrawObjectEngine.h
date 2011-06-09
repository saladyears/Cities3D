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

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class IGameObject;
typedef boost::shared_ptr<IGameObject> GameObjectPtr;

class IDrawObject;
typedef boost::shared_ptr<IDrawObject> DrawObjectPtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: IDrawObjectEngine
// 
// The interface of the <IDrawObject> engine.  The IDrawObjectEngine
// maintains a database of <IDrawObjects> that have been provided by
// the loaded <RuleSets>.  Whenever a new <IGameObject> is created by
// an <IRule>, the IDrawObjectEngine is called via <CreateDrawObject>,
// to create an <IDrawObject> for the new <IGameObject> so that it 
// can be drawn on screen.  IDrawObjectEngine also has two functions,
// <ObtainAll> and <ReleaseAll>, that allow all stored <IDrawObjects> in
// the database to do their own resource acquisition and release.  These 
// functions must be called exactly once per Cities3D game.  The 
// <IDrawObjectEngine> is accessible through the <GameEngine>.
//
// Project:
//     <RulesCore>
//
// Include:
//     IDrawObjectEngine.h
//
class RULES_EXPORT_SPEC IDrawObjectEngine
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: CreateDrawObject
	//
	// Creates a new <IDrawObject> capable of drawing the given
	// <IGameObject>.
	//
	// Parameters:
	//     object - The <IGameObject> for which an <IDrawObject> is needed.
	//
	// Returns:
	//     A newly cloned <IDrawObject> that can draw the <IGameObject>.  
	//     May be NULL if no capable <IDrawObject> was found.
	//
	virtual DrawObjectPtr CreateDrawObject(
		const GameObjectPtr &object) const=0;

	//-----------------------------------------------------------------------//
	// Function: ObtainAll
	//
	// Lets all <IDrawObjects> obtain needed resources.
	//
	virtual void ObtainAll()=0;

	//-----------------------------------------------------------------------//
	// Function: ReleaseAll
	//
	// Lets all <IDrawObjects> release acquired resources.
	//
	virtual void ReleaseAll()=0;

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~IDrawObjectEngine
	// 
	// The IDrawObjectEngine destructor.  It is pure virtual because
	// this class must be derived from in order to be used.
	// 
	virtual ~IDrawObjectEngine()=0;
};

//---------------------------- PROTOTYPES       -----------------------------//
inline IDrawObjectEngine::~IDrawObjectEngine() {}
