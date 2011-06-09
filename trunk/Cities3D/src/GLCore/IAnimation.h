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
#include "GLExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: IAnimation
//
// The interface for all game animations.
// 
// Project:
//     <GLCore>
//
// Include:
//     IAnimation.h
//
class GL_EXPORT_SPEC IAnimation
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Animate
	//
	// Runs the animation.
	//
	virtual void Animate()=0;

	//-----------------------------------------------------------------------//
	// Function: IsDone
	//
	// Returns whether the animation is done or not.
	//
	// Returns:
	//     A bool that is true if the animation is done and false if not.
	//
	virtual const bool IsDone() const=0;

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~IAnimation
	//
	// The IAnimation destructor.
	//
	virtual ~IAnimation() {};
};

//---------------------------- PROTOTYPES       -----------------------------//

typedef boost::shared_ptr<IAnimation> AnimationPtr;
