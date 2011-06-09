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
#include "ILogic.h"
#include "Access.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: Logic
// 
// The default implementation of <ILogic> for normal <Game> actions.  The Logic 
// class inherits from both <ILogic> and <Access>, so that all logic derived 
// from Logic has access to the <Game> data that the <GameEngine> provides.
//
// Derived From:
//     <ILogic>
//     <Access>
//
// Project:
//     <RulesCore>
//
// Include:
//     Logic.h
//
class RULES_EXPORT_SPEC Logic : public ILogic, public Access
{
	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: Logic
	//
	// The default Logic constructor.  Does nothing, but is
	// required so that on Win32, the .DLL file exports
	// something that can be linked to.
	//
	Logic();
};

//---------------------------- DEFINES          -----------------------------//

//---------------------------- PROTOTYPES       -----------------------------//

