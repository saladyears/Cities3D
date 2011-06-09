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
#include "IAnimation.h"
#include "GLExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: Animation
// 
// The default implementation of the <IAnimation> interface.
//
// Derived From:
//     <IAnimation>
//
// Project:
//     <GLCore>
//
// Include:
//     Animation.h
//
class GL_EXPORT_SPEC Animation : public IAnimation
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Animate
	//
	// Runs the animation.  Determines how far into the animation we are based
	// on the current time and calls AnimatePrivate with the percentage through
	// the animation if needed.
	//
	virtual void Animate();

	//-----------------------------------------------------------------------//
	// Function: IsDone
	//
	// Returns whether the animation is done or not.
	//
	// Returns:
	//     A bool that is true if the animation is done and false if not.
	//
	virtual const bool IsDone() const;

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: Animation
	//
	// The Animation constructor.
	//
	// Parameters:
	//     duration - The duration of the Animation in milliseconds.
	//     start - The start time of the Animation in milliseconds.
	//
	Animation(const long duration, const long start = 0);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~Animation
	//
	// The Animation destructor.  Ensures that the animation leaves itself
	// at 100% complete.
	//
	~Animation();

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: AnimatePrivate
	//
	// The private animation implementation.
	//
	// Parameters:
	//     percent - The percentage through the animation to play.
	//
	virtual void AnimatePrivate(const float percent) const=0;

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Int: mStart
	//
	// The start time of the Animation.
	//
	long mStart;

	//-----------------------------------------------------------------------//
	// Int: mDuration
	//
	// The duration of the Animation.
	//
	long mDuration;

	//-----------------------------------------------------------------------//
	// Object: mStopWatch
	//
	// The stopwatch that governs how the Animation knows how much time has
	// passed.
	//
	wxStopWatch mStopWatch;
};

//---------------------------- PROTOTYPES       -----------------------------//

