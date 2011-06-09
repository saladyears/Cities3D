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
#include "RulesExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: ISoundEngine
// 
// The interface of the sound engine.  The ISoundEngine
// plays the given sound file when called.  It is
// responsible for managing whatever underlying sound
// engine implementation is used.  The <ISoundEngine> is 
// accessible through the <GameEngine>.
//
// Project:
//     <RulesCore>
//
// Include:
//     ISoundEngine.h
//
class RULES_EXPORT_SPEC ISoundEngine
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: PlaySound
	//
	// Plays the given sound.  The sound name must be the
	// name of a sound file in the /sounds directory or one
	// of its subdirectories.  If the sound cannot be played,
	// an error message is thrown to the screen.
	//
	// Parameters:
	//     sound - The name of the sound file to play.
	//
	virtual void PlaySound(const wxString &sound) const=0;

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~ISoundEngine
	// 
	// The ISoundEngine destructor.  It is pure virtual because
	// this class must be derived from in order to be used.
	// 
	virtual ~ISoundEngine()=0;
};

//---------------------------- PROTOTYPES       -----------------------------//
inline ISoundEngine::~ISoundEngine() {}
