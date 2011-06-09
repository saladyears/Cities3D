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
#include <boost/weak_ptr.hpp>

//---------------------------- USER INCLUDES    -----------------------------//
#include "GameObject.h"
#include "ModelExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: CornerObject
//
// A base class for objects that sit on the corners of hexes.  There
// are many <GameObjects> such as settlements, cities, knights, etc.,
// that sit on the intersection of three hexes.  CornerObject provides
// a base class for them all since they share much in common.
//
// Derived From:
//     <GameObject>
//
// Project:
//     <ModelCore>
//
// Include:
//     CornerObject.h
//
class MODEL_EXPORT_SPEC CornerObject : public GameObject
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~CornerObject
	//
	// The CornerObject destructor.  It is pure virtual because
	// this class must be derived from to be used.
	//
	virtual ~CornerObject()=0;

	//-----------------------------------------------------------------------//
	// Group: Operators
	//

	//-----------------------------------------------------------------------//
	// Function: operator==
	//
	// Compares another CornerObject to this one to see if
	// they are the same.  All three tile/corner combinations
	// and the color must be equal for the objects to be
	// the same.
	//
	// Returns:
	//     A bool giving the equality of the given object.  True
	//     if the object is the same, false if not.
	//
	bool operator==(const CornerObject &object) const;

	typedef std::pair<wxInt32, wxInt32> TileCorner;

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: bridge
	//
	// Returns whether this CornerObject acts as a bridge between <SideObjects>
	// of different types.
	//
	// Returns:
	//     A bool that is true if the object acts as a bridge between
	//     <SideObjects> and false if not.
	//
	virtual bool bridge() const
	{
		return true;
	}

	//-----------------------------------------------------------------------//
	// Function: tileCorner
	//
	// Sets a new tile and corner for the object.
	//
	// Parameters:
	//     tile - The new tile.
	//     corner - The new corner.
	//     game - The current <Game>.
	//
	void tileCorner(wxInt32 tile, wxInt32 corner, const GamePtr& game);

	//-----------------------------------------------------------------------//
	// Function: tile1
	//
	// Returns the first tile and corner the object is on.
	//
	// Returns:
	//     A std::pair of the first tile and corner the object is on.
	//
	TileCorner tile1() const {return mTile1;}

	//-----------------------------------------------------------------------//
	// Function: tile2
	//
	// Returns the second tile and corner the object is on.
	//
	// Returns:
	//     A std::pair of the second tile and corner the object is on.
	//
	TileCorner tile2() const {return mTile2;}

	//-----------------------------------------------------------------------//
	// Function: tile3
	//
	// Returns the third tile and corner the object is on.
	//
	// Returns:
	//     A std::pair of the third tile and corner the object is on.
	//
	TileCorner tile3() const {return mTile3;}

	//-----------------------------------------------------------------------//
	// Function: links
	//
	// Returns the number of objects linked to this one.
	//
	// Returns:
	//     The number of objects linked to this one.
	//
	wxInt32 links() const;

	//-----------------------------------------------------------------------//
	// Function: above
	//
	// Sets the object above this one.
	//
	// Parameters:
	//     The object to set.
	//
	void above(boost::weak_ptr<CornerObject> above)
	{
		mAbove = above;
	}

	//-----------------------------------------------------------------------//
	// Function: above
	//
	// Returns the object above this one.
	//
	// Returns:
	//     The object above to this one.  May be NULL.
	//
	boost::weak_ptr<CornerObject> above() const
	{
		return mAbove;
	}

	//-----------------------------------------------------------------------//
	// Function: below
	//
	// Sets the object below this one.
	//
	// Parameters:
	//     The object to set.
	//
	void below(boost::weak_ptr<CornerObject> below)
	{
		mBelow = below;
	}

	//-----------------------------------------------------------------------//
	// Function: below
	//
	// Returns the object below this one.
	//
	// Returns:
	//     The object below to this one.  May be NULL.
	//
	boost::weak_ptr<CornerObject> below() const
	{
		return mBelow;
	}

	//-----------------------------------------------------------------------//
	// Function: onTile
	//
	// Returns whether the CornerObject is on the given tile.
	//
	// Returns:
	//     A bool that is whether the CornerObject is on the
	//     given tile.  The value is true if it is, false if not.
	//
	bool onTile(wxInt32 tile);

	//-----------------------------------------------------------------------//
	// Function: onTileCorner
	//
	// Returns whether the CornerObject is on the given tile
	// and corner.  Checks all three values against the tile.
	//
	// Returns:
	//     A bool that is whether the CornerObject is on the
	//     given tile.  The value is true if it is, false if not.
	//
	bool onTileCorner(const TileCorner tile);

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
	// Loads a CornerObject from an input stream.
	//
	// Parameters:
	//     stream - The input stream.
	//
	virtual void load(wxDataInputStream &stream);

	//-----------------------------------------------------------------------//
	// Function: save
	//
	// Writes a CornerObject to an output stream.
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
	// Constructor: CornerObject
	//
	// The CornerObject constructor.  Finds the neighboring
	// tiles from the given tile and sets the coordinates for the
	// object based on the tile and corner.
	//
	// Parameters:
	//     owner - The owner of the object.
	//     turn - The turn the object is created on.
	//     color - The color of this object.  Should be some player's color.
	//     game - The current <Game> object.
	//     tile - The tile the object is on.
	//     corner - The corner on the tile the object is on.  From 0 to 5 
	//              inclusive.
	//     above - The object, if any, above this CornerObject.
	//
	CornerObject(wxInt32 owner, wxInt32 turn, const ColorType color, 
		const GamePtr &game, const wxInt32 tile, const wxInt32 corner,
		boost::weak_ptr<CornerObject> above = boost::weak_ptr<CornerObject>());

	//-----------------------------------------------------------------------//
	// Constructor: CornerObject
	//
	// The default CornerObject constructor.
	//
	CornerObject();

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
	// The first tile/corner this object is on.
	//
	TileCorner mTile1;

	//-----------------------------------------------------------------------//
	// Object: mTile2
	//
	// The second tile/corner this object is on.
	//
	TileCorner mTile2;

	//-----------------------------------------------------------------------//
	// Object: mTile3
	//
	// The third tile/corner this object is on.
	//
	TileCorner mTile3;

	//-----------------------------------------------------------------------//
	// Object: mColor
	//
	// The color of this object.
	//
	ColorType mColor;

	//-----------------------------------------------------------------------//
	// Object: mAbove
	//
	// The link to the CornerObject above this one, if any.
	//
	boost::weak_ptr<CornerObject> mAbove;

	//-----------------------------------------------------------------------//
	// Object: mAbove
	//
	// The link to the CornerObject below this one, if any.
	//
	boost::weak_ptr<CornerObject> mBelow;
};

//---------------------------- PROTOTYPES       -----------------------------//
