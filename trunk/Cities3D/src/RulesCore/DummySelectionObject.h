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
#include "SelectionObject.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: DummySelectionObject
// 
// A <SelectionObject> for that does not need an <IDrawObject>.  Most
// types of selection, for tiles, settlements, knights, etc., don't actually
// need a separate draw object to handle the selection possibilities, since
// the IGameObject::Select flag suffices to set them as selectable.
// The class DummySelectionObject provides a way for the Model to still
// signal to the View that a selection needs to happen, but that the
// visual representation of the selection will be taken care of by the
// objects themselves.  The class <DrawDummySelectionObject> is the
// <IDrawObject>-derived class that "draws" the DummySelectionObject (by
// doing nothing).
//
// Derived From:
//     <SelectionObject>
//
// Project:
//     <RulesCore>
//
// Include:
//     DummySelectionObject.h
//

class RULES_EXPORT_SPEC DummySelectionObject : public SelectionObject
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: DummySelectionObject 
	//
	// The DummySelectionObject constructor.  Stores the
	// given rule name.  See <SelectionObject> for details.
	//
	DummySelectionObject(const HashString &rule);
	
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Clone
	//
	// Clones the HexSelectionObject.  *Unused*.
	//
	// Returns:
	//     A NULL pointer.
	//
	virtual IGameObject *Clone() {return NULL;}

	//-----------------------------------------------------------------------//
	// Function: name
	//
	// Resets the class name of the DummySelectionObject.
	//
	// Returns:
	//     A const reference to a string containing the
	//     class name of the DummySelectionObject.
	//
	virtual const HashString& name() 
	{
		return shDummySelectionObject;
	}
};

//---------------------------- PROTOTYPES       -----------------------------//
