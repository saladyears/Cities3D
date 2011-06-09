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
#include "IRule.h"
#include "Access.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: Rule
// 
// The default implementation of <IRule> for normal <Game> actions.  The Rule 
// class inherits from both <IRule> and <Access>, so that all rules derived 
// from Rule have access to the <Game> data that the <GameEngine> provides.
//
// Derived From:
//     <IRule>
//     <Access>
//
// Project:
//     <RulesCore>
//
// Include:
//     Rule.h
//
class RULES_EXPORT_SPEC Rule : public IRule, public Access
{
	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: Rule
	//
	// The default Rule constructor.  Does nothing, but is required so that on 
	// Win32, the .DLL file exports something that can be linked to.
	//
	Rule();
};

//---------------------------- DEFINES          -----------------------------//

//---------------------------- PROTOTYPES       -----------------------------//

