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
#include "Data.h"
#include "GameObject.h"
#include "ModelExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class MapDatabase;

class ChipObject;
typedef boost::shared_ptr<ChipObject> ChipObjectPtr;

class PortObject;
typedef boost::shared_ptr<PortObject> PortObjectPtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: TileObject
// 
// Represents one hexagonally-shaped tile on the board.  The board
// itself is made up of an array of TileObjects.  All TileObjects have an
// index, a roll value (which may be -1 to indicate no roll), port
// corners (if the tile is a port), a port side (if the tile has 
// a fixed orientation), a border flag (if the tile is one of the
// edges of the map), a tile type, and an array of integers that
// are the indices of other tiles that surround the tile.  A -1 along a tile's
// side indicates that there is no tile bordering there (i.e., it is the
// edge of the map).
//
// Other plugins may add more data fields to a tile based on their needs,
// by using the extensibility of the hash-map based <Data> class.
//
// Derived From:
//     <Data>
//     <GameObject>
//
// Project:
//     <ModelCore>
//
// Include:
//     TileObject.h
//
class MODEL_EXPORT_SPEC TileObject : public Data, public GameObject
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: TileObject
	//
	// The TileObject constructor.  A TileObject should
	// always have a valid index.
	//
	// Parameters:
	//     index - The index of this TileObject.
	//
	TileObject(wxInt32 index = -1);

	//-----------------------------------------------------------------------//
	// Group: Enumerations
	//

	//-----------------------------------------------------------------------//
	// Enum: DrawFlags
	//
	// Custom draw flags for a TileObject.  Available custom draw
	// flags are: *OutlineRed* (outlines a tile in red)
	//
	enum DrawFlags
	{
		OutlineRed		= 0,

		DrawFlagsMax	= 16,
		DrawFlagsDword	= 0x7fffffff
	};

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: chip
	//
	// Returns the ChipObject that lays atop this tile.  May
	// be NULL if none exists.
	//
	// Returns:
	//     A const reference to a ChipObject shared_ptr.
	//
	const ChipObjectPtr &chip() const {return mChipObject;}

	//-----------------------------------------------------------------------//
	// Function: port
	//
	// Returns the PortObject that lays atop this tile.  May
	// be NULL if none exists.
	//
	// Returns:
	//     A const reference to a PortObject shared_ptr.
	//
	const PortObjectPtr &port() const {return mPortObject;}

	//-----------------------------------------------------------------------//
	// Function: randomizeRotation
	//
	// Randomizes the rotation of this tile.  The tile is 
	// rotated in multiples of 60 degrees.
	//	
	// This function must be called before the ports are calculated 
	// for the board, or else the port orientations will become 
	// improperly aligned.
	//
	void randomizeRotation();

	//-----------------------------------------------------------------------//
	// Function: createChipObject
	//
	// Creates or destroys the <ChipObject> for this TileObject.
	// Only one <ChipObject> can exist at a time on a TileObject.
	//
	// Parameters:
	//     show - A flag to determine whether the <ChipObject> should
	//            be created or destroyed.  If true, the <ChipObject>
	//            should be created.  If false, destroyed.
	//
	void createChipObject(bool show);

	//-----------------------------------------------------------------------//
	// Function: createPortObject
	//
	// Creates or destroys the <PortObject> for this TileObject.
	// Only one <PortObject> can exist at a time on a TileObject.
	//
	// Parameters:
	//     show - A flag to determine whether the <PortObject> should
	//            be created or destroyed.  If true, the <PortObject>
	//            should be created.  If false, destroyed.
	//
	void createPortObject(bool show);

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Clone
	//
	// Clones the TileObject.  *Unused*.
	//
	// Returns:
	//     A NULL pointer.
	//
	virtual IGameObject *Clone() {return NULL;}

	//-----------------------------------------------------------------------//
	// Function: name
	//
	// Resets the class name of the TileObject.
	//
	// Returns:
	//     A const reference to a string containing the
	//     class name of the TileObject.
	//
	virtual const HashString& name() 
	{
		return shTileObject;
	}

	//-----------------------------------------------------------------------//
	// Function: reset
	//
	// Resets the display flags.  Also resets display
	// flags for any <ChipObject> or <PortObject>
	// on the tile.
	//
	virtual void reset();

	//-----------------------------------------------------------------------//
	// Function: set
	//
	// Turns a specific display flag on.  Also turns
	// the flag on for any <ChipObject> or <PortObject>
	// on the tile.
	//
	// Parameters:
	//     flag - The display flag to turn on.
	//
	virtual void set(const ObjectFlag flag);

	// Provide access to base set function.
	using GameObject::set;

	//-----------------------------------------------------------------------//
	// Function: load
	//
	// Loads a TileObject from an input stream.
	//
	// Parameters:
	//     stream - The input stream.
	//
	virtual void load(wxDataInputStream &stream);

	//-----------------------------------------------------------------------//
	// Function: save
	//
	// Writes a TileObject to an output stream.
	//
	// Parameters:
	//     stream - The output stream.
	//
	virtual void save(wxDataOutputStream &stream) const;

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Pointer: mChipObject
	//
	// The <ChipObject> that lays atop this TileObject.
	// May be NULL.
	//
	ChipObjectPtr mChipObject;

	//-----------------------------------------------------------------------//
	// Pointer: mPortObject
	//
	// The <PortObject> that lays atop this TileObject.
	// May be NULL.
	//
	PortObjectPtr mPortObject;

	//allow tile access to rules
	friend class OfficialMap;
	friend class Access;
};
typedef boost::shared_ptr<TileObject> TileObjectPtr;

//---------------------------- PROTOTYPES       -----------------------------//
