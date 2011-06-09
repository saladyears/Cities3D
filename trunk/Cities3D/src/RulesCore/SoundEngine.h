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
#include "ISoundEngine.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

#ifdef FMOD4
// fmod4 forward declarations
namespace FMOD {
	class System;
}
#endif

//---------------------------------------------------------------------------//
// Class: SoundEngine
// 
// The default implementation of the <ISoundEngine> interface.  
// Plays sounds when requested using the fmod sound engine.
// See <http://www.fmod.org> for full documentation of the
// fmod sound engine.
//
// Derived From:
//     <ISoundEngine>
//
// Project:
//     <RulesCore>
//
// Include:
//     SoundEngine.h
//
class RULES_EXPORT_SPEC SoundEngine : public ISoundEngine
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: SoundEngine
	// 
	// The SoundEngine constructor.  The constructor
	// initializes the fmod sound engine so that it is ready
	// to play sounds.
	// 
	SoundEngine();

#	ifdef FMOD4
	//-----------------------------------------------------------------------//
	// Destructor: ~SoundEngine
	//
	// The SoundEngine deconstructor. Releases the fmod system.
	// It currently is only defined when FMOD4 is used.
	~SoundEngine();
#	endif

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: PlaySound
	//
	// Plays a sound immediately.  If the given sound is
	// not valid, or does not exist, an error log message
	// is displayed in the main window with the appropriate
	// information.
	//
	// Parameters:
	//     sound - The full path to the sound file.  Sound files
	//             may be .wav, .mp3, or various other file formats.
	//             See <http://www.fmod.org> for more information.
	//
	void PlaySound(const wxString &sound) const;

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
#	ifdef FMOD4
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Pointer: mSystem
	//
	// A pointer to a FMOD4 system.
	// It is only available, if FMOD4 is defined.
	//
	FMOD::System *mSystem;
#	endif
};

//---------------------------- PROTOTYPES       -----------------------------//

