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
#include "IGameObject.h"
#include "ModelExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class TileObject;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: BaseTiledObject
// 
// Common base class for objects that lay on top of a tile. A BaseTiledObject 
// is created with a pointer to the tile on which it lays, and the tile can be 
// directly queried to provide game information any derived class does not want 
// to provide.  Derived classes may also provide their own special information 
// that assists in drawing them on the screen.
//
// Derived From:
//     <IGameObject>
//
// Project:
//     <ModelCore>
//
// Include:
//     BaseTiledObject.h
//
class MODEL_EXPORT_SPEC BaseTiledObject : public IGameObject
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:	
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Clone
	//
	// Clones the PortObject.  *Unused*.
	//
	// Returns:
	//     A NULL pointer.
	//
	virtual IGameObject *Clone() {return NULL;}

	//-----------------------------------------------------------------------//
	// Function: reset
	//
	// Resets the display flags.
	//
	virtual void reset() {mDisplayFlags = 0;}

	//-----------------------------------------------------------------------//
	// Function: isset
	//
	// Reports if a specific display flag is set.
	//
	// Parameters:
	//     flag - The display flag being checked.
	//
	// Returns:
	//     A boolean value saying whether the flag is set.  The 
	//     value is true if the flag is set.
	virtual bool isset(const ObjectFlag flag) const;

	//-----------------------------------------------------------------------//
	// Function: isset
	//
	// Reports if a custom display flag is set.
	//
	// Returns:
	//     A boolean value saying whether the flag is set.  The 
	//     value is true if the flag is set.
	//
	virtual bool isset(const wxInt32 flag) const;

	//-----------------------------------------------------------------------//
	// Function: set
	//
	// Turns a specific display flag on.
	//
	// Parameters:
	//     flag - The display flag to turn on.
	//
	virtual void set(const ObjectFlag flag);

	//-----------------------------------------------------------------------//
	// Function: set
	//
	// Turns a custom display flag on.
	//
	// Parameters:
	//     flag - The display flag to turn on.
	//
	virtual void set(const wxInt32 flag);
	
	//-----------------------------------------------------------------------//
	// Function: id
	//
	// Provides the ID of the BaseTiledObject.  The ID is retrieved
	// from the <TileObject>.
	//
	// Returns:
	//     The ID of the BaseTiledObject.
	//
	virtual wxInt32 id() const;

	//-----------------------------------------------------------------------//
	// Function: id
	//
	// Sets the ID of the BaseTiledObject.  This function is not implemented.
	// It is only declared to satisfy the requirements of <IGameObject>.
	//
	// Parameters:
	//     id - The new ID.
	//
	virtual void id(const wxInt32) {}

	//-----------------------------------------------------------------------//
	// Function: uicoords
	//
	// Provides the UI coordinates of the object.
	//
	// Returns:
	//     The ui coordinates of the object.
	//
	virtual Vector uicoords() const
	{
		return Vector(0.0f, 0.0f, 0.0f);
	}

	//-----------------------------------------------------------------------//
	// Function: coords
	//
	// Provides the coordinates of the BaseTiledObject.  The coordinates
	// are retrieved from the <TileObject>.
	//
	// Returns:
	//     The coordinates of the BaseTiledObject.
	//
	virtual const Vector &coords() const;

	//-----------------------------------------------------------------------//
	// Function: coords
	//
	// Sets the coordinates of the BaseTiledObject.  This function is not 
	// implemented. It is only declared to satisfy the requirements of 
	// <IGameObject>.
	//
	// Parameters:
	//     coords - The new coordinates.
	//
	virtual void coords(const Vector &) {}

	//-----------------------------------------------------------------------//
	// Function: color
	//
	// Provides the color of the BaseTiledObject.  Since having a
	// color on a BaseTiledObject is nonsensical, this function
	// returns CR_SIZE, which is no color at all.
	//
	// Returns:
	//     A nonsensical color value.
	//
	virtual ColorType color() const {return CR_SIZE;}

	//-----------------------------------------------------------------------//
	// Function: color
	//
	// Sets the color of the BaseTiledObject.  This function is not 
	// implemented. It is only declared to satisfy the requirements of 
	// <IGameObject>.
	//
	// Parameters:
	//     color - The new color.
	//
	virtual void color(const ColorType) {}

	//-----------------------------------------------------------------------//
	// Function: owner
	//
	// Returns the owner of this IGameObject.
	//
	// Returns:
	//     The index of the owner, or -1 if no one owns the object.
	//
	virtual wxInt32 owner() const
	{
		// BaseTiledObjects aren't owned by anyone.
		return -1;
	}

	//-----------------------------------------------------------------------//
	// Function: turn
	//
	// Returns the turn number the object was created on, or -1 if it always
	// exists.
	//
	// Returns:
	//     The turn number the object was createdon, or -1 if it always exists.
	//
	virtual wxInt32 turn() const
	{
		// BaseTiledObject always exist.
		return -1;
	}

	//-----------------------------------------------------------------------//
	// Function: load
	//
	// Loads a BaseTiledObject from an input stream.
	//
	// Parameters:
	//     stream - The input stream.
	//
	virtual void load(wxDataInputStream &stream);

	//-----------------------------------------------------------------------//
	// Function: save
	//
	// Writes a BaseTiledObject to an output stream.
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
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: BaseTiledObject
	//
	// The BaseTiledObject constructor.
	//
	// Parameters:
	//     pTile - A pointer to the <TileObject> the BaseTiledObject
	//             is built on and will refer to for game data.
	//
	BaseTiledObject(const TileObject * const pTile);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~BaseTiledObject
	//
	// The BaseTiledObject destructor.  It is pure virtual
	// because this class must be derived from to be used.
	//
	virtual ~BaseTiledObject()=0;

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Pointer: mpTile
	//
	// The <TileObject> the chip lays upon, from which it draws 
	// its location, roll value, and id.
	const TileObject * const mpTile;

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Int: mDisplayFlags
	//
	// The object display flags.
	//
	wxInt32 mDisplayFlags;
};

//---------------------------- PROTOTYPES       -----------------------------//
