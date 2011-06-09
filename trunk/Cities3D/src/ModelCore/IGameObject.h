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
#include <math.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "ModelExport.h"
#include "Vector.h"
#include "ColorInfoData.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxDataInputStream;
class wxDataOutputStream;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: IGameObject
// 
// An interface for all game objects.  Any object in the game must
// have certain properties.  These properties include: having
// flags that indicate how the object should be drawn, a way to modify
// those flags, an ID for selection, coordinates in world space,
// a rotation, and any color that should be applied to the object.
//
// Project:
//     <ModelCore>
//
// Include:
//     IGameObject.h
//
class MODEL_EXPORT_SPEC IGameObject
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~IGameObject
	//
	// The IGameObject destructor.  It is pure virtual because
	// this class must be derived from to be used.
	//
	virtual ~IGameObject()=0;

	//-----------------------------------------------------------------------//
	// Group: Enumerations
	//

	//-----------------------------------------------------------------------//
	// Enum: ObjectFlag
	//
	// Standard draw flags for a game object.  Available draw
	// flags include: *Dim* (dims the object), *Outline* (draws the
	// object with an outline), *Select* (allows the object to be
	// selected), *Hidden* (shows/hides the object).
	//
	enum ObjectFlag
	{
		Dim = 0,
		Outline,
		Select,
		Hidden,

		//NOTE: there may not be more than 16 ObjectFlags, so that all
		//flags and custom flags may be stored in one 32-bit integer
		MaxFlag = 16,

		ObjectDword = 0x7ffffff
	};

	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Clone
	//
	// Clones the <IGameObject>.
	//
	// Returns:
	//     A pointer to a new instance of the object.
	//
	virtual IGameObject *Clone()=0;

	//-----------------------------------------------------------------------//
	// Function: name
	//
	// Resets the class name of the IGameObject.
	//
	// Returns:
	//     A const reference to a string containing the
	//     class name of the IGameObject.
	virtual const HashString& name()=0;

	//-----------------------------------------------------------------------//
	// Function: reset
	//
	// Resets the display flags.
	//
	virtual void reset()=0;

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
	virtual bool isset(const ObjectFlag flag) const=0;

	//-----------------------------------------------------------------------//
	// Function: isset
	//
	// Reports if a custom display flag is set.
	//
	// Returns:
	//     A boolean value saying whether the flag is set.  The 
	//     value is true if the flag is set.
	//
	virtual bool isset(const wxInt32 flag) const=0;

	//-----------------------------------------------------------------------//
	// Function: set
	//
	// Turns a specific display flag on.
	//
	// Parameters:
	//     flag - The display flag to turn on.
	//
	virtual void set(const ObjectFlag flag)=0;

	//-----------------------------------------------------------------------//
	// Function: set
	//
	// Turns a custom display flag on.
	//
	// Parameters:
	//     flag - The display flag to turn on.
	//
	virtual void set(const wxInt32 flag)=0;
	
	//-----------------------------------------------------------------------//
	// Function: id
	//
	// Provides the ID of the object.
	//
	// Returns:
	//     The ID of the object.
	//
	virtual wxInt32 id() const=0;

	//-----------------------------------------------------------------------//
	// Function: id
	//
	// Sets the ID of the object.
	//
	// Parameters:
	//     id - The new ID.
	//
	virtual void id(const wxInt32 id)=0;

	//-----------------------------------------------------------------------//
	// Function: uicoords
	//
	// Provides the UI coordinates of the object.
	//
	// Returns:
	//     The ui coordinates of the object.
	//
	virtual Vector uicoords() const=0;

	//-----------------------------------------------------------------------//
	// Function: coords
	//
	// Provides the coordinates of the object.
	//
	// Returns:
	//     The coordinates of the object.
	//
	virtual const Vector &coords() const=0;

	//-----------------------------------------------------------------------//
	// Function: coords
	//
	// Sets the coordinates of the object.
	//
	// Parameters:
	//     coords - The new coordinates.
	//
	virtual void coords(const Vector &coords)=0;

	//-----------------------------------------------------------------------//
	// Function: rotation
	//
	// Provides the rotation of the object.
	//
	// Returns:
	//     The rotation of the object.
	//
	virtual const Vector &rotation() const=0;

	//-----------------------------------------------------------------------//
	// Function: rotation
	//
	// Sets the rotation of the object.
	//
	// Parameters:
	//     val - The new rotation.
	//
	virtual void rotation(const Vector &coords)=0;

	//-----------------------------------------------------------------------//
	// Function: color
	//
	// Provides the color of the object.
	//
	// Returns:
	//     The color of the object.
	//
	virtual ColorType color() const=0;

	//-----------------------------------------------------------------------//
	// Function: color
	//
	// Sets the color of the object.
	//
	// Parameters:
	//     color - The new color.
	//
	virtual void color(const ColorType color)=0;

	//-----------------------------------------------------------------------//
	// Function: owner
	//
	// Returns the owner of this IGameObject.
	//
	// Returns:
	//     The index of the owner, or -1 if no one owns the object.
	//
	virtual wxInt32 owner() const=0;

	//-----------------------------------------------------------------------//
	// Function: turn
	//
	// Returns the turn number the object was created on, or -1 if it always
	// exists.
	//
	// Returns:
	//     The turn number the object was createdon, or -1 if it always exists.
	//
	virtual wxInt32 turn() const=0;

	//-----------------------------------------------------------------------//
	// Function: load
	//
	// Loads an IGameObject from an input stream.
	//
	// Parameters:
	//     stream - The input stream.
	//
	virtual void load(wxDataInputStream &stream)=0;

	//-----------------------------------------------------------------------//
	// Function: save
	//
	// Writes an IGameObject to an output stream.
	//
	// Parameters:
	//     stream - The output stream.
	//
	virtual void save(wxDataOutputStream &stream) const=0;
};

typedef boost::shared_ptr<IGameObject> GameObjectPtr;

//---------------------------- PROTOTYPES       -----------------------------//
inline IGameObject::~IGameObject() {}
