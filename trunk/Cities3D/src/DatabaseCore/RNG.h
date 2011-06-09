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
#include "DatabaseExport.h"
#include "Singleton.h"

//---------------------------- DEFINES          -----------------------------//
#define RAND RNG::get()

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: RNG
// 
// The random number generator (RNG).  The RNG provides both a pool of random 
// numbers for the game to use with functions that allow synchronization for 
// network games, and a method to get unpooled ranom numbers for values that
// have no need of synchronization.
//
// Project:
//     <DatabaseCore>
//
// Derived From:
//     <Singleton>
//
// Include:
//     RNG.h
//
class DATABASE_EXPORT_SPEC RNG : public Singleton<RNG>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: repool
	//
	// Clears the current random number pool and refills it.
	//
	void repool();

	//-----------------------------------------------------------------------//
	// Function: unpooled
	//
	// Returns an unsigned integer between 0 and n - 1, inclusive, from the 
	// internal random number generator.
	//
	// Parameters:
	//     n - The upper bound of the desired unsigned integer value.
	//
	// Returns:
	//     An unsigned integer that is between 0 and n - 1, inclusive.
	//
	const wxUint32 unpooled(const wxUint32 n) const;

	//-----------------------------------------------------------------------//
	// Function: pooled
	//
	// Returns an unsigned integer between 0 and n - 1, inclusive, from the
	// pool of random numbers.
	//
	// Parameters:
	//     n - The upper bound of the desired unsigned integer value.
	//
	// Returns:
	//     An unsigned integer that is between 0 and n - 1, inclusive.
	//
	const wxUint32 pooled(const wxUint32 n) const;

	//-----------------------------------------------------------------------//
	// Function: load
	//
	// Loads the RNG pool from an input stream.
	//
	// Parameters:
	//     stream - The input stream.
	//
	void load(wxDataInputStream &stream);

	//-----------------------------------------------------------------------//
	// Function: save
	//
	// Writes the RNG pool to an output stream.
	//
	// Parameters:
	//     stream - The output stream.
	//
	void save(wxDataOutputStream &stream) const;

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: RNG
	//
	// The RNG constructor.  Initializes the internal pool of random numbers.
	//
	RNG();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Group: next
	//
	// Returns the next value in the internal pool and updates the index.
	//
	// Returns:
	//     An unsigned integer that is the next value in the pool.
	//
	const wxUint32 next() const;

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Int: mIndex;
	//
	// The current index into the pool of random number values.
	//
	mutable wxUint32 mIndex;

	//-----------------------------------------------------------------------//
	// Array: mPool
	//
	// The internal pool of randomly generated values.
	//
	boost::scoped_array<wxUint32> mPool;

	friend class Singleton<RNG>;
};

//---------------------------- PROTOTYPES       -----------------------------//

