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
// Class: SideObject
//
// A base class for objects that sit on the sides of hexes.  There
// are several <GameObjects> such as roads and ships that sit on the
// sides of two hexes.  SideObject provides a base class for them 
// all since they share much in common.
//
// Derived From:
//     <GameObject>
//
// Project:
//     <ModelCore>
//
// Include:
//     SideObject.h
//
class MODEL_EXPORT_SPEC SideObject : public GameObject
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~SideObject
	//
	// The SideObject destructor.  It is pure virtual because
	// this class must be derived from to be used.
	//
	virtual ~SideObject()=0;

	typedef std::pair<wxInt32, wxInt32> TileSide;

	//-----------------------------------------------------------------------//
	// Group: Operators
	//

	//-----------------------------------------------------------------------//
	// Function: operator==
	//
	// Compares another SideObject to this one to see if
	// they are the same.  Both tile/corner combinations
	// and the color must be equal for the objects to be
	// the same.
	//
	// Returns:
	//     A bool giving the equality of the given object.  True
	//     if the object is the same, false if not.
	//
	bool operator==(const SideObject &object) const;

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: tile1
	//
	// Returns the first tile and side the object is on.
	//
	// Returns:
	//     A std::pair of the first tile and side the object is on.
	//
	TileSide tile1() const {return mTile1;}

	//-----------------------------------------------------------------------//
	// Function: tile2
	//
	// Returns the second tile and side the object is on.
	//
	// Returns:
	//     A std::pair of the second tile and side the object is on.
	//
	TileSide tile2() const {return mTile2;}
	
	//-----------------------------------------------------------------------//
	// Function: onTileSide
	//
	// Returns whether the SideObject is on the given tile
	// and side.  Checks both values against the tile.
	//
	// Returns:
	//     A bool that is whether the SideObject is on the
	//     given tile.  The value is true if it is, false if not.
	//
	bool onTileSide(const TileSide tile)
	{
		return 
			(mTile1 == tile) ||
			(mTile2 == tile);
	}

	//-----------------------------------------------------------------------//
	// Function: longest
	//
	// Returns whether the SideObject is part of Longest Road.
	//
	// Returns:
	//     A bool that is true if the SideObject is part of Longest Road and
	//     false if not.
	//
	bool longest() const
	{
		return mLongest;
	}

	//-----------------------------------------------------------------------//
	// Function: longest
	//
	// Sets the Longest Road flag for the SideObject.
	//
	// Parameters:
	//     longest - A flag that is true if the SideObject is part of Longest
	//               Road and false if not.
	//
	void longest(bool longest)
	{
		mLongest = longest;
	}

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: color
	//
	// Returns the color of the object.
	//
	// Returns:
	//     The color of the object.
	//
	virtual ColorType color() const {return mColor;}

	//-----------------------------------------------------------------------//
	// Function: color
	//
	// Sets the color of the object.
	//
	// Parameters:
	//     color - The new color of the object.
	//
	virtual void color(const ColorType color) {mColor = color;}

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
		return mOwner;
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
		return mTurn;
	}

	//-----------------------------------------------------------------------//
	// Function: load
	//
	// Loads a SideObject from an input stream.
	//
	// Parameters:
	//     stream - The input stream.
	//
	virtual void load(wxDataInputStream &stream);

	//-----------------------------------------------------------------------//
	// Function: save
	//
	// Writes a SideObject to an output stream.
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
	// Constructor: SideObject
	//
	// The CornerObject constructor.  Finds the neighboring
	// tile from the given tile and sets the coordinates for the
	// object based on the tile and side.
	//
	// Parameters:
	//     owner - The owner of the object.
	//     turn - The turn the object is created on.
	//     color - The color of this object.  Should be some player's color.
	//     game - The current <Game> object.
	//     tile - The tile the object is on.
	//     side - The side on the tile the object is on.  From 0 to 5 
	//            inclusive.
	//
	SideObject(wxInt32 owner, wxInt32 turn, const ColorType color, 
		const GamePtr &game, const wxInt32 tile, const wxInt32 side);

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Object: mOwner
	//
	// The owner of this object.
	//
	wxInt32 mOwner;

	//-----------------------------------------------------------------------//
	// Object: mTurn
	//
	// The turn the object was created on.
	//
	wxInt32 mTurn;

	//-----------------------------------------------------------------------//
	// Object: mTile1
	//
	// The first tile/side this object is on.
	//
	TileSide mTile1;

	//-----------------------------------------------------------------------//
	// Object: mTile2
	//
	// The second tile/side this object is on.
	//
	TileSide mTile2;
	
	//-----------------------------------------------------------------------//
	// Object: mColor
	//
	// The color of this object.
	//
	ColorType mColor;

	//-----------------------------------------------------------------------//
	// Bool: mLongest
	//
	// Whether this object is part of a chain of Longest Road.
	//
	bool mLongest;
};

//---------------------------- PROTOTYPES       -----------------------------//
inline SideObject::~SideObject() {}
