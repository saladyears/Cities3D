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
#include "AnimationRing.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	const float sOuterRadius = 2.0f;
	const float sThickness = 0.50f;
}

//---------------------------- PUBLIC           -----------------------------//
AnimationRing::AnimationRing(boost::weak_ptr<IGameObject> object, 
	const long duration, const long start)
: AnimationGameObject(object, duration, start)
{
	mQuad = gluNewQuadric();
}

AnimationRing::~AnimationRing()
{
	gluDeleteQuadric(mQuad);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void AnimationRing::AnimatePrivate(const float percent) const
{
	GameObjectPtr object = GetGameObject();

	if(object)
	{
		const Vector &vec = object->coords();

		float outerRadius = sOuterRadius * (1.0f - percent);
		float innerRadius = outerRadius - sThickness;

		// Clamp.
		innerRadius = std::max(innerRadius, 0.0f);

		glPushMatrix();
			glTranslated(vec.x, 0.15f, vec.z);
			glRotated(90.0, -1.0, 0.0, 0.0);
			gluDisk(mQuad, innerRadius, outerRadius, 25, 5);
		glPopMatrix();
	}
}
