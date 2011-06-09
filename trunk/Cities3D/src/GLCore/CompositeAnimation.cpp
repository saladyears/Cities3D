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
#include "CompositeAnimation.h"
#include "DrawObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
void CompositeAnimation::Animate()
{
	mColor.UseMaterial();

	std::for_each(mAnimationSet.begin(), mAnimationSet.end(),
		boost::bind(&IAnimation::Animate, _1));

	DrawObject::material(DrawObject::White).UseMaterial();
}

const bool CompositeAnimation::IsDone() const
{
	bool done = true;

	AnimationSet::const_iterator it, itEnd = mAnimationSet.end();
	for(it = mAnimationSet.begin(); it != itEnd; ++it)
	{
		if(false == (*it)->IsDone())
		{
			done = false;
			break;
		}
	}
	
	return done;
}

//---------------------------- PROTECTED        -----------------------------//
CompositeAnimation::CompositeAnimation(const ColorType color)
{
	mColor = DrawObject::material(color);	
}

void CompositeAnimation::AddAnimation(const AnimationPtr &animation)
{
	mAnimationSet.insert(animation);
}

//---------------------------- PRIVATE          -----------------------------//
