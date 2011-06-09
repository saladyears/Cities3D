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
#include "GameObject.h"
#include "ModelExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: HexObject
//
// A base class for objects that sit on hexes directly.  There
// are several <GameObjects> such as the robber, pirate, merchant,
// etc., which sit on a hex directly.  HexObject provides a base class
// for them all since they share much in common.
//
// Derived From:
//     <GameObject>
//
// Project:
//     <ModelCore>
//
// Include:
//     HexObject.h
//
class MODEL_EXPORT_SPEC HexObject : public GameObject
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~HexObject
	//
	// The HexObject destructor.  It is pure virtual because
	// this class must be derived from to be used.
	//
	virtual ~HexObject()=0;

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: tile
	//
	// Sets the tile the object is on.
	//
	// Parameters:
	//     tile - The new tile for the object.
	//
	void tile(wxInt32 tile) {mTile = tile;}

	//-----------------------------------------------------------------------//
	// Function: tile
	//
	// Returns the tile the object is on.
	//
	// Returns:
	//     The tile the object is on.
	//
	wxInt32 tile() const {return mTile;}

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: load
	//
	// Loads a HexObject from an input stream.
	//
	// Parameters:
	//     stream - The input stream.
	//
	virtual void load(wxDataInputStream &stream);

	//-----------------------------------------------------------------------//
	// Function: save
	//
	// Writes a HexObject to an output stream.
	//
	// Parameters:
	//     stream - The output stream.
	//
	virtual void save(wxDataOutputStream &stream) const;

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructor
	//

	//-----------------------------------------------------------------------//
	// Constructor: HexObject
	//
	// The HexObject constructor.  Sets the tile the
	// object is on to the given tile.
	//
	// Parameters:
	//     tile - The tile the object is on.
	//
	HexObject(const wxInt32 tile);

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Int: mTile
	//
	// The tile the object is on.
	//
	wxInt32 mTile;
};

//---------------------------- PROTOTYPES       -----------------------------//
inline HexObject::~HexObject() {}
