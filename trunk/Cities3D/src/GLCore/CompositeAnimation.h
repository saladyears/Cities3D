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
#include <set>

//---------------------------- USER INCLUDES    -----------------------------//
#include "IAnimation.h"
#include "GLExport.h"
#include "ColorInfoData.h"
#include "GLMaterial.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: CompositeAnimation
//
// An <IAnimation> that is made up of several sub <Animations>.
// 
// Derived From:
//     <IAnimation>
//
// Project:
//     <GLCore>
//
// Include:
//     CompositeAnimation.h
//
class GL_EXPORT_SPEC CompositeAnimation : public IAnimation
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Function: Animate
	//
	// Runs all of the child animations.
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
	// Constructor: CompositeAnimation
	//
	// The CompositeAnimation constructor.
	//
	CompositeAnimation(const ColorType color);

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: AddAnimation
	//
	// Adds the given <Animation> to the internal animation set.
	//
	// Parameters:
	//     animation - The <Animation> to add.
	//
	void AddAnimation(const AnimationPtr &animation);

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//
	
	typedef std::set<AnimationPtr> AnimationSet;

	//-----------------------------------------------------------------------//
	// Object: mAnimationSet
	//
	// The set of <Animations> to run.
	//
	AnimationSet mAnimationSet;

	//-----------------------------------------------------------------------//
	// Object: mColor
	//
	// The color to use for the Animation.
	//
	GLMaterial mColor;
};

//---------------------------- PROTOTYPES       -----------------------------//

