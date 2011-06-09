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
#include "RulesExport.h"
#include "ColorInfoData.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: SelectionObject
// 
// The base class for all selection-related <IGameObjects>.  
// Whenever an <IRule> requires that the user provide input to 
// continue, for instance, when selecting a location for a
// settlement or road, it must pass a SelectionObject up to the
// View via the eventSelectionObject <Event> in the <Controller>.
// The SelectionObject provides a rule name and color for any
// special selection drawing.  The rule name is the next <IRule>
// to execute when the user makes a valid selection.  When
// an object (or whatever) is selected in the View, it queries
// the SelectionObject for the name of its <IRule>, then calls
// the <IRuleEngine> with that <IRule> name, passing the ID of
// whatever object was selected as the <DataObject>.  The <IRule>
// then proceeds with the given information.
//
// In this way, the View knows nothing at all about the specifics
// of object selection.  It simply reports the ID of any <IGameObject>
// selected to the <IRule> stored in the SelectionObject.
//
// Derived From:
//     <GameObject>
//
// Project:
//     <RulesCore>
//
// Include:
//     SelectionObject.h
//

class RULES_EXPORT_SPEC SelectionObject : public GameObject
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: rule
	//
	// Returns the name of the <IRule> to call when an object
	// is selected.
	//
	// Returns:
	//     The name of the <IRule> to call.
	//
	const HashString &rule() 
	{
		return mRule;
	}

	//-----------------------------------------------------------------------//
	// Function: color
	//
	// Returns the color any special selection drawing should
	// be done in.
	//
	// Returns:
	//     The color for special selection drawing.
	//
	ColorType color() {return mColor;}

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: SelectionObject
	//
	// The SelectionObject constructor.  Stores the given
	// rule name and color.
	//
	// Parameters:
	//     rule - The name of the <IRule> to execute when an
	//            object is selected.
	//     color - The color any selection drawing should be done in.
	//
	SelectionObject(const HashString &rule, const ColorType color) 
		: mRule(rule), mColor(color) {}

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~SelectionObject
	// 
	// The SelectionObject destructor.  It is pure virtual because
	// this class must be derived from in order to be used.
	// 
	virtual ~SelectionObject()=0;

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// String: mRule
	//
	// The name of the <IRule> that should be called with the selection
	// ID once something has been selected.
	//
	HashString mRule;

	//-----------------------------------------------------------------------//
	// Object: mColor
	//
	// The color that the selection objects should be drawn in.
	//
	ColorType mColor;
};

typedef boost::shared_ptr<SelectionObject> SelectionObjectPtr;

//---------------------------- PROTOTYPES       -----------------------------//
inline SelectionObject::~SelectionObject() {}
