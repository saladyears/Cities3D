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
#include "AnimationGameObject.h"
#include "GLExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: AnimationRing
//
// An <Animation> that draws a ring animating inward.
//
// Derived From:
//     <Animation>
//
// Project:
//     <GLCore>
//
// Include:
//     AnimationRing.h
//
class GL_EXPORT_SPEC AnimationRing : public AnimationGameObject
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: AnimationRing
	//
	// The AnimationRing constructor.
	//
	// Parameters:
	//     object - The <IGameObject> this Animation is linked to.
	//     duration - The duration of the Animation in milliseconds.
	//     start - The start time of the Animation in milliseconds.
	//
	AnimationRing(boost::weak_ptr<IGameObject> object, 
		const long duration, const long start = 0);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~AnimationRing
	//
	// The AnimationRing destructor.
	//
	~AnimationRing();

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: AnimatePrivate
	//
	// The private animation implementation.
	//
	// Parameters:
	//     percent - The percentage through the animation to play.
	//
	virtual void AnimatePrivate(const float percent) const;

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Pointer: mQuad
	//
	// The OpenGL quadric object used for drawing the ring.
	//
	GLUquadricObj *mQuad;
};

//---------------------------- PROTOTYPES       -----------------------------//

