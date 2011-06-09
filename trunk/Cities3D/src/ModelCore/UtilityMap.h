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
#include "ModelExport.h"

//---------------------------- DEFINES          -----------------------------//
#include "Data.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Namespace: UtilityMap
// 
// A namespace that provides miscellaneous utility functions
// for working with map data.
//
// Project:
//     <DatabaseCore>
//
// Include:
//     UtilityMap.h
//
namespace UtilityMap
{
	//-----------------------------------------------------------------------//
	// Function: prepareDataArray
	//
	// Fills up a string array based on the given fill types.  This
	// is useful for creating the undetermined parts of maps at runtime.
	// The given string array is filled with each of the keys in the
	// fill types, in the quantity of the number assigned to that key.
	//
	// Parameters:
	//     fillTypes - The integer hash of values to fill the array with.  
	//                 See <Data> for details.
	//     values - The string array to be filled with data.
	//
	// Returns:
	//     The number of values inserted into the string array.
	//
	wxInt32 MODEL_EXPORT_SPEC prepareDataArray(const Data::IntHash &fillTypes, 
		HashStringArray &values);
}

//---------------------------- PROTOTYPES       -----------------------------//
