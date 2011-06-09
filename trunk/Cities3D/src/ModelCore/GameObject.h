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
#include "IGameObject.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxDataInputStream;
class wxDataOutputStream;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: GameObject
// 
// A default implementation of the <IGameObject> interface.  GameObject
// provides a default class for handling all of the functionality needed
// in a game object.  Most objects in the game should derive from it.  It
// fulfills all of the requirements of the <IGameObject> interface.
//
// Derived From:
//     <IGameObject>
//
// Project:
//     <ModelCore>
//
// Include:
//     GameObject.h
//
class MODEL_EXPORT_SPEC GameObject : public IGameObject
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructor
	//

	//-----------------------------------------------------------------------//
	// Constructor: GameObject
	//
	// The GameObject constructor.  Initializes all values
	// to 0 or their equivalent.
	//
	GameObject();

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

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
	// Provides the ID of the object.
	//
	// Returns:
	//     The ID of the object.
	//
	virtual wxInt32 id() const {return mID;}

	//-----------------------------------------------------------------------//
	// Function: id
	//
	// Sets the ID of the object.
	//
	// Parameters:
	//     id - The new ID.
	//
	virtual void id(const wxInt32 id) {mID = id;}

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
	// Provides the coordinates of the object.
	//
	// Returns:
	//     The coordinates of the object.
	//
	virtual const Vector &coords() const {return mCoords;}

	//-----------------------------------------------------------------------//
	// Function: coords
	//
	// Sets the coordinates of the object.
	//
	// Parameters:
	//     coords - The new coordinates.
	//
	virtual void coords(const Vector &val) {mCoords = val;}

	//-----------------------------------------------------------------------//
	// Function: rotation
	//
	// Provides the rotation of the object.
	//
	// Returns:
	//     The rotation of the object.
	//
	virtual const Vector& rotation() const {return mRotation;}

	//-----------------------------------------------------------------------//
	// Function: rotation
	//
	// Sets the rotation of the object.
	//
	// Parameters:
	//     val - The new rotation.
	//
	virtual void rotation(const Vector& val) {mRotation = val;}

	//-----------------------------------------------------------------------//
	// Function: color
	//
	// Provides the color of the object.  Not implemented
	// in this class, but provided with a non-sensical return
	// value so that the requirements of IGameObject are satisfied.
	// Derived classes can override this function if they require
	// color settings.
	//
	// Returns:
	//     A non-sensical color value.
	//
	virtual ColorType color() const {return CR_SIZE;}

	//-----------------------------------------------------------------------//
	// Function: color
	//
	// Sets the color of the object.  Not implemented in this
	// class, but provided so that the requirements of IGameObject
	// are satisfied.  Derived classes can override this function
	// if they require color settings.
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
		// Defaults to -1.
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
		// Defaults to -1.
		return -1;
	}

	//-----------------------------------------------------------------------//
	// Function: load
	//
	// Loads a GameObject from an input stream.
	//
	// Parameters:
	//     stream - The input stream.
	//
	void load(wxDataInputStream &stream);

	//-----------------------------------------------------------------------//
	// Function: save
	//
	// Writes a GameObject to an output stream.
	//
	// Parameters:
	//     stream - The output stream.
	//
	void save(wxDataOutputStream &stream) const;

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Int: mDisplayFlags
	//
	// The object display flags.
	//
	wxInt32 mDisplayFlags;

	//-----------------------------------------------------------------------//
	// Int: mID
	//
	// The object ID.
	//
	wxInt32 mID;

	//-----------------------------------------------------------------------//
	// Object: mCoords
	//
	// The coordinates in world space.
	//
	Vector mCoords;

	//-----------------------------------------------------------------------//
	// Float: mRotation
	//
	// The local rotation of the object.
	//
	Vector mRotation;	
};

//---------------------------- PROTOTYPES       -----------------------------//
