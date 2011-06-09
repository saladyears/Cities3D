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
#include "Animation.h"
#include "GLExport.h"
#include "IGameObject.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: AnimationGameObject
// 
// An <Animation> based off an <IGameObject> and a player's color..
//
// Derived From:
//     <Animation>
//
// Project:
//     <GLCore>
//
// Include:
//     AnimationGameObject.h
//
class GL_EXPORT_SPEC AnimationGameObject : public Animation
{
	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: AnimationGameObject
	//
	// The AnimationGameObject constructor.
	//
	// Parameters:
	//     object - The <IGameObject> this Animation is linked to.
	//     duration - The duration of the Animation in milliseconds.
	//     start - The start time of the Animation in milliseconds.
	//
	AnimationGameObject(boost::weak_ptr<IGameObject> object, 
		const long duration, const long start = 0)
		: Animation(duration, start)
		, mGameObject(object)
	{}

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: GetGameObject
	//
	// Returns a boost::shared_ptr to the <IGameObject> this Animation is
	// linked to.
	//
	boost::shared_ptr<IGameObject> GetGameObject() const
	{
		return mGameObject.lock();
	}

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Pointer: mGameObject
	//
	// The <IGameObject> this Animation is linked to.
	//
	boost::weak_ptr<IGameObject> mGameObject;
};

//---------------------------- PROTOTYPES       -----------------------------//

