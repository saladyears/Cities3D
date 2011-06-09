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
#include <wx/datstrm.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "BaseTiledObject.h"
#include "ModelExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: ChipObject
// 
// Stores game information for a roll chip laying on top of a tile. Since the 
// only important information for a chip is its roll value and any rotation 
// applied to it, all other game data is retrieved by querying the <TileObject> 
// that the ChipObject is built on.
//
// Derived From:
//     <BaseTiledObject>
//
// Project:
//     <ModelCore>
//
// Include:
//     ChipObject.h
//
class MODEL_EXPORT_SPEC ChipObject : public BaseTiledObject
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: ChipObject
	//
	// The ChipObject constructor.
	//
	// Parameters:
	//     pTile - A pointer to the <TileObject> the ChipObject
	//             is built on and will refer to for non-chip game data.
	//
	ChipObject(const TileObject * const pTile = NULL);
	
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: roll
	//
	// Reads the roll value from the <TileObject>.
	// The roll value is needed for rendering the chip in OpenGL
	// with the right texture.
	//
	// Returns:
	//     The roll value of the ChipObject.
	//
	wxInt32 roll() const;

	//-----------------------------------------------------------------------//
	// Function: randomizeRotation
	//
	// Randomizes the rotation of both this chip.  The chip is 
	// rotated from 0 to 360 degrees.
	//	
	void randomizeRotation();

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: rotation
	//
	// Provides the rotation of the ChipObject.
	//
	// Returns:
	//     The rotation of the ChipObject.
	//
	virtual const Vector& rotation() const {return mRotation;}

	//-----------------------------------------------------------------------//
	// Function: rotation
	//
	// Sets the rotation of the ChipObject.
	//
	// Parameters:
	//     val - The new rotation.
	//
	virtual void rotation(const Vector& val) {mRotation = val;}

	//-----------------------------------------------------------------------//
	// Function: load
	//
	// Loads a ChipObject from an input stream.
	//
	// Parameters:
	//     stream - The input stream.
	//
	virtual void load(wxDataInputStream &stream);

	//-----------------------------------------------------------------------//
	// Function: save
	//
	// Writes a ChipObject to an output stream.
	//
	// Parameters:
	//     stream - The output stream.
	//
	virtual void save(wxDataOutputStream &stream) const;

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: name
	//
	// Resets the class name of the ChipObject.
	//
	// Returns:
	//     A const reference to a string containing the
	//     class name of the ChipObject.
	//
	virtual const HashString& name() 
    {
        return shChipObject;
    }

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Float: mRotation
	//
	// The rotation applied to the chip.
	//
	Vector mRotation;
};

//---------------------------- PROTOTYPES       -----------------------------//
