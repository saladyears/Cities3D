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

//---------------------------- TYPEDEFS         -----------------------------//
class wxDataInputStream;
class wxDataOutputStream;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: ISerialize
// 
// Interface for serialization. 
//
// Project:
//     <ModelCore>
//
// Include:
//     ISerialize.h
//
class MODEL_EXPORT_SPEC ISerialize
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~ISerialize
	//
	// The ISerialize destructor.  It is pure virtual because
	// this class must be derived from to be used.
	//
	virtual ~ISerialize()=0;

	//-----------------------------------------------------------------------//
	// Function: load
	//
	// Loads from an input stream.
	//
	// Parameters:
	//     stream - The input stream.
	//
	virtual void load(wxDataInputStream &stream)=0;

	//-----------------------------------------------------------------------//
	// Function: save
	//
	// Writes to an output stream.
	//
	// Parameters:
	//     stream - The output stream.
	//
	virtual void save(wxDataOutputStream &stream) const=0;
};

//---------------------------- PROTOTYPES       -----------------------------//

