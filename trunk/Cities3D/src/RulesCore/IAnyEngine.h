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
#include "RulesExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class IAnySerializer;
typedef boost::shared_ptr<IAnySerializer> IAnySerializerPtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: IAnyEngine
// 
// Needs documentation.
//
// Project:
//     <RulesCore>
//
// Include:
//     IAnyEngine.h
//
class RULES_EXPORT_SPEC IAnyEngine
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: AddAnySerializer
	//
	// Adds an <IAnySerializer> to the internal store.
	//
	// Parameters:
	//     serializer - The <IAnySerializer> to add.
	//
	virtual void AddAnySerializer(const IAnySerializerPtr &serializer)=0;

	//-----------------------------------------------------------------------//
	// Function: RemoveAnySerializer
	//
	// Removes an <IAnySerializer> to the internal store.
	//
	// Parameters:
	//     serializer - The <IAnySerializer> to remove.
	//
	virtual void RemoveAnySerializer(const IAnySerializerPtr &serializer)=0;

	//-----------------------------------------------------------------------//
	// Function: FindAnySerializerByType
	//
	// Returns the <IAnySerializer> belonging to the given type string.  A
	// valid object must be returned.
	//
	// Parameters:
	//     type - The type of the desired <IAnySerializer>.
	//
	// Returns:
	//     An <IAnySerializer> that can serialize the given type.
	//
	virtual const IAnySerializerPtr &FindAnySerializerByType(
		const HashString &type) const=0;

	//-----------------------------------------------------------------------//
	// Function: FindAnySerializerByTypeID
	//
	// Returns the <IAnySerializer> belonging to the given type id.
	// If no <IAnySerializer> exists for the given type id, an empty
	// object is returned.
	//
	// Parameters:
	//     type - The type of the desired <IAnySerializer>.
	//
	// Returns:
	//     An <IAnySerializer> that can serialize the given type, or
	//     NULL if none exists.
	//
	virtual const IAnySerializerPtr &FindAnySerializerByTypeID(
		const HashString &typeID) const=0;

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~IAnyEngine
	// 
	// The IAnyEngine destructor.  It is pure virtual because
	// this class must be derived from in order to be used.
	// 
	virtual ~IAnyEngine()=0;
};

//---------------------------- PROTOTYPES       -----------------------------//
inline IAnyEngine::~IAnyEngine() {}
