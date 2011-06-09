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
#include "Vector.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: Box
//
// An axis-aligned bounding box.
//
// Project:
//     <ModelCore>
//
// Include:
//     Box.h
//
class Box
{
public:
	Box(const Vector &p1, const Vector &p2)
		: mP1(p1), mP2(p2)
	{
		wxASSERT(p1 < p2);
	}

	// Test the box to see if it is intersected by the given ray.  If it is,
	// set the distance from the ray that the intersection takes place.
	bool TestIntersection(const Vector &origin, const Vector &ray)
	{
		// Taken from http://www.cs.princeton.edu/~rmorley/pubs/williams.pdf.
		// This is a fast algorithm for determining if a ray hits an axis-
		// aligned bounding box.
		float tmin, tmax;

		float div = 1.0f / ray.x;

		if (div >= 0.0f) 
		{
			tmin = (mP1.x - origin.x) * div;
			tmax = (mP2.x - origin.x) * div;
		}
		else 
		{
			tmin = (mP2.x - origin.x) * div;
			tmax = (mP1.x - origin.x) * div;
		}

		div = 1.0f / ray.y;

		float tymin, tymax;

		if (div >= 0) 
		{
			tymin = (mP1.y - origin.y) * div;
			tymax = (mP2.y - origin.y) * div;
		}
		else 
		{
			tymin = (mP2.y - origin.y) * div;
			tymax = (mP1.y - origin.y) * div;
		}

		if ((tmin > tymax) || 
			(tymin > tmax))
		{
			return false;
		}

		if (tymin > tmin)
		{
			tmin = tymin;
		}

		if (tymax < tmax)
		{
			tmax = tymax;
		}
		
		div = 1.0f / ray.z;

		float tzmin, tzmax;

		if (div >= 0) 
		{
			tzmin = (mP1.z - origin.z) * div;
			tzmax = (mP2.z - origin.z) * div;
		}
		else 
		{
			tzmin = (mP2.z - origin.z) * div;
			tzmax = (mP1.z - origin.z) * div;
		}

		if ((tmin > tzmax) || 
			(tzmin > tmax))
		{
			return false;
		}

		return true;
	}

private:
	// A box is made up of two points in space that define the opposite
	// corners.
	Vector mP1;
	Vector mP2;
};

//---------------------------- PROTOTYPES       -----------------------------//

