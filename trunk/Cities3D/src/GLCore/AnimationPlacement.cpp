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
#include "stdwx.h"
#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//
#include "AnimationPlacement.h"
#include "AnimationRing.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
AnimationPlacement::AnimationPlacement(const GameObjectPtr &object, 
									   const ColorType color)
: CompositeAnimation(color)
{
	AddAnimation(AnimationPtr(new AnimationRing(object, 750)));
	AddAnimation(AnimationPtr(new AnimationRing(object, 750, 600)));
	AddAnimation(AnimationPtr(new AnimationRing(object, 750, 1200)));
	AddAnimation(AnimationPtr(new AnimationRing(object, 750, 1800)));
	AddAnimation(AnimationPtr(new AnimationRing(object, 750, 2400)));	
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
