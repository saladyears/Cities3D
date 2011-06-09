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
#include <math.h>

//---------------------------- USER INCLUDES    -----------------------------//

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Struct: Vector
// 
// A lightweight 1x3 vector class.  Vector allows game
// objects to be represented in three dimensional world space.
// It provides basic arithmetic and vector functions for 
// manipulating position data.
//
// Project:
//     <ModelCore>
//
// Include:
//     Vector.h
//
struct Vector
{
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: Vector
	//
	// The default Vector constructor.  Initializes all
	// values to zero.
	//
	Vector() 
		: x(0.0f), y(0.0f), z(0.0f) {}

	//-----------------------------------------------------------------------//
	// Constructor: Vector
	//
	// The Vector constructor.  Initializes all
	// values to the given values.
	//
	// Parameters:
	//     a - The x value.
	//     b - The y value.
	//     c - The z value.
	//
	Vector(float a, float b, float c) 
		: x(a), y(b), z(c) {}

	//-----------------------------------------------------------------------//
	// Group: Operators
	//

	//-----------------------------------------------------------------------//
	// Function: operator==
	//
	// Compares the vector to another vector.
	//
	// Parameters:
	//     rhs - The vector to compare.
	//
	// Returns:
	//     A bool that is true if the Vectors are equal and
	//     false if not.
	//
	bool operator==(const Vector &rhs)
	{
		return ( (x == rhs.x) &&
				 (y == rhs.y) &&
				 (z == rhs.z));
	}

	//-----------------------------------------------------------------------//
	// Function: operator!=
	//
	// Compares the vector to another vector.
	//
	// Parameters:
	//     rhs - The vector to compare.
	//
	// Returns:
	//     A bool that is false if the Vectors are equal and
	//     true if not.
	//
	bool operator!=(const Vector &rhs)
	{
		return !operator==(rhs);
	}

	//-----------------------------------------------------------------------//
	// Function: operator-=
	//
	// Subtracts another vector from the vector.
	//
	// Parameters:
	//     rhs - The vector to subtract.
	//
	Vector &operator-=(const Vector &rhs) 
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;

		return *this;
	}

	//-----------------------------------------------------------------------//
	// Function: operator+=
	//
	// Adds another vector to the vector.
	//
	// Parameters:
	//     rhs - The vector to add.
	//
	Vector &operator+=(const Vector &rhs) 
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;

		return *this;
	}

	//-----------------------------------------------------------------------//
	// Function: operator*=
	//
	// Multiples the vector by a scalar.
	//
	// Parameters:
	//     rhs - The scalar to multiply by.
	//
	Vector &operator*=(const float rhs)
	{
		//optimizations
		if(0.0 == rhs)
		{
			*this = Vector();
		}
		else if(1.0 != rhs)
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
		}

		return *this;
	}

	//-----------------------------------------------------------------------//
	// Function: operator/=
	//
	// Divides the vector by a scalar.
	//
	// Parameters:
	//     rhs - The scalar to divide by.
	//
	Vector &operator/=(const float rhs) 
	{
		//sanity
		if(0.0 == rhs)
		{
			return *this;
		}

		float o = 1.0f / rhs; 
		
		x *= o;
		y *= o;
		z *= o;
		
		return *this;
	}
	
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Length
	//
	// Calculates and returns the length of the vector.
	//
	// Returns:
	//     The length of the vector.
	//
	float Length() const 
	{
		return sqrt(x * x + y * y + z * z);
	}

	//-----------------------------------------------------------------------//
	// Function: Normalize
	//
	// Normalizes the vector in place.
	//
	void Normalize()
	{
		float len = Length();

		if(0.0 == len)
		{
			return;
		}

		float oneOverLen = 1.0f / len;
		x *= oneOverLen;
		y *= oneOverLen;
		z *= oneOverLen;
	}

	//-----------------------------------------------------------------------//
	// Function: Normalize
	//
	// Normalizes the vector into the given Vector.
	//
	// Parameters:
	//     dest - The destination Vector.
	//
	void Normalize(Vector &dest)
	{
		dest = Vector(*this);
		dest.Normalize();
	}

	float x;
	float y;
	float z;
};

inline Vector CrossProduct(const Vector &lhs, const Vector &rhs)
{
	//use temporary return value compiler optimization
	return Vector((lhs.y * rhs.z) - (lhs.z * rhs.y),
				  (lhs.z * rhs.x) - (lhs.x * rhs.z),
				  (lhs.x * rhs.y) - (lhs.y * rhs.x));
}

inline float DotProduct(const Vector &lhs, const Vector &rhs)
{
	//use temporary return value compiler optimization
	return ((lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z));
}

inline const Vector operator+(const Vector &lhs, const Vector &rhs)
{
	Vector ret(lhs);
	ret += rhs;

	return ret;
}

inline const Vector operator-(const Vector &lhs, const Vector &rhs)
{
	Vector ret(lhs);
	ret -= rhs;

	return ret;
}

inline const Vector operator*(const Vector &lhs, const float rhs)
{
	Vector ret(lhs);
	ret *= rhs;

	return ret;
}

inline bool operator<(const Vector &lhs, const Vector &rhs)
{
	return ((lhs.x < rhs.x) && (lhs.y < rhs.y) && (lhs.z < rhs.z));
}

//---------------------------- PROTOTYPES       -----------------------------//
