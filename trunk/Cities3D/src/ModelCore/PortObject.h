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
class TileObject;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: PortObject
// 
// Stores game information for a port laying on top of a tile.
// Since the only important information for a port is what
// type of port it is, all other information for the PortObject
// is retrieved by querying its <TileObject>.
//
// Derived From:
//     <BaseTiledObject>
//
// Project:
//     <ModelCore>
//
// Include:
//     PortObject.h
//
class MODEL_EXPORT_SPEC PortObject : public BaseTiledObject
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: PortObject
	//
	// The PortObject constructor.
	//
	// Parameters:
	//     pTile - A pointer to the <TileObject> the PortObject
	//             is built on and will refer to for game data.
	//
	PortObject(const TileObject * const pTile = NULL);

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: type
	//
	// Reads the port type value from the <TileObject>.
	// The port type is needed for rendering the chip in OpenGL
	// with the right texture.
	//
	// Returns:
	//     A string containing the type of the PortObject.
	//
	const HashString &type() const;

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: name
	//
	// Resets the class name of the PortObject.
	//
	// Returns:
	//     A const reference to a string containing the
	//     class name of the PortObject.
	//
	virtual const HashString& name() 
	{
		return shPortObject;
	}

	//-----------------------------------------------------------------------//
	// Function: rotation
	//
	// Provides the rotation of the PortObject.
	//
	// Returns:
	//     The rotation of the PortObject.
	//
	virtual const Vector& rotation() const;

	//-----------------------------------------------------------------------//
	// Function: rotation
	//
	// Sets the rotation of the PortObject. This function is not implemented.
	// It is only declared to satisfy the requirements of <IGameObject>.
	//
	// Parameters:
	//     val - The new rotation.
	//
	virtual void rotation(const Vector& val);
};

//---------------------------- PROTOTYPES       -----------------------------//
