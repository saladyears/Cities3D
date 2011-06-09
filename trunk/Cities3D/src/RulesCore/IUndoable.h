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

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: IUndoable
// 
// The interface for undoable <IRules>.  Some <IRules>
// should be undoable by the user.  By implementing this
// interface, an <IRule> can declare that it has the possibility
// of being undone.  The <CanUndo> function then gives the
// actual undoableness of the <IRule> when queried.
//
// Project:
//     <RulesCore>
//
// Include:
//     IUndoable.h
//
class RULES_EXPORT_SPEC IUndoable
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: CanUndo
	//
	// Returns whether the <IRule> can be undone.
	//
	// Returns:
	//     A bool that states if the <IRule> can be undone.
	//     The value is true if it can be, false if not.
	//
	virtual bool CanUndo() const=0;

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~IUndoable
	// 
	// The IUndoable destructor.  It is pure virtual because
	// this class must be derived from in order to be used.
	// 
	virtual ~IUndoable()=0;
};

//---------------------------- PROTOTYPES       -----------------------------//
inline IUndoable::~IUndoable() {}

