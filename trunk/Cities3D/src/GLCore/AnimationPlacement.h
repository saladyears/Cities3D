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
#include "CompositeAnimation.h"
#include "GLExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class IGameObject;
typedef boost::shared_ptr<IGameObject> GameObjectPtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: AnimationPlacement
//
// An <Animation> that draws consecutive rings underneath a newly placed item.
//
// Derived From:
//     <CompositeAnimation>
//
// Project:
//     <GLCore>
//
// Include:
//     AnimationPlacement.h
//
class GL_EXPORT_SPEC AnimationPlacement : public CompositeAnimation
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: AnimationPlacement
	//
	// The AnimationPlacement constructor.
	//
	// Parameters:
	//     object - The newly placed <IGameObject>.
	//     color - The color of the placement.
	//
	AnimationPlacement(const GameObjectPtr &object, const ColorType color);
};

//---------------------------- PROTOTYPES       -----------------------------//

