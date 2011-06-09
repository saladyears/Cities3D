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
#include "IDrawObjectEngine.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: DrawObjectEngine
// 
// The standard implementation of the <IDrawObjectEngine> interface.
// DrawObjectEngine stores all of the <IDrawObjects> provided by
// the loaded <RuleSets> internally in a std::list container.  The
// functions required by the <IDrawObjectEngine> interface are implemented
// by taking the std::list and performing the necessary operations.
// See the function documentation below for more details.
//
// Derived From:
//     <IDrawObjectEngine>
//
// Project:
//     <RulesCore>
//
// Include:
//     DrawObjectEngine.h
//

class RULES_EXPORT_SPEC DrawObjectEngine : public IDrawObjectEngine
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
	// Stops the DrawObjectEngine.  Clears the std::list of
	// <IDrawObjects>.
	//
	void Stop();

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: CreateDrawObject
	//
	// Creates a new <IDrawObject> capable of drawing the given
	// <IGameObject>.  Searches the std::list of <IDrawObjects>, calling
	// <CanBuild> on each with the given <IGameObject> until an <IDrawObject>
	// is found that can build it.  If one is found, the new <IDrawObject>
	// is created using the prototype's <Clone> function and then
	// returned.  If a handler can't be found, an empty object is
	// returned.
	//
	// Parameters:
	//     object - The <IGameObject> for which an <IDrawObject> is needed.
	//
	// Returns:
	//     A newly cloned <IDrawObject> that can draw the <IGameObject>.  
	//     May be NULL if no capable <IDrawObject> was found.
	//
	virtual DrawObjectPtr CreateDrawObject(const GameObjectPtr &object) const;

	//-----------------------------------------------------------------------//
	// Function: ObtainAll
	//
	// Lets all <IDrawObjects> obtain needed resources.  Calls the
	// Obtain function on each <IDrawObject> in the std::list.
	//
	virtual void ObtainAll();

	//-----------------------------------------------------------------------//
	// Function: ReleaseAll
	//
	// Lets all <IDrawObjects> release acquired resources.  Calls the
	// Release function on each <IDrawObject> in the std::list.
	//
	virtual void ReleaseAll();

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	typedef std::list<DrawObjectPtr> DrawObjectPtrList;

	//-----------------------------------------------------------------------//
	// Variable: mObjectList
	//
	// The std::list of <IDrawObjects> available to the game.
	//
	DrawObjectPtrList mObjectList;

	friend class PluginEngine;
};

//---------------------------- PROTOTYPES       -----------------------------//
