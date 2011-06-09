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
#include "stdwx.h"
#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//
#include "SoundEngine.h"
#include "FilePath.h"
#include "ConfigDatabase.h"

#ifdef FMOD4
#include "fmod.hpp"
#else
#include "fmod.h"
#endif
#include "fmod_errors.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
SoundEngine::SoundEngine()
#ifdef FMOD4
: mSystem(NULL)
#endif
{
#	ifdef FMOD4
	FMOD_RESULT result;

	if(FMOD_OK != (result = FMOD::System_Create(&mSystem)))
	{
		wxLogError(wxT("Can't create sound system: %s"),
			FMOD_ErrorString(result));

		mSystem = NULL;
		return;
	}

#	ifdef __linux__
	// set output to alsa
	if(FMOD_OK != (result = mSystem->setOutput(FMOD_OUTPUTTYPE_ALSA)))
	{
		wxLogError(wxT("Can't set output to alsa: %s"),
			FMOD_ErrorString(result));
	}
#	endif // __linux__

	result = mSystem->init(32, FMOD_INIT_NORMAL, 0);
	if(FMOD_OK != result)
	{
		mSystem->close();
		mSystem->release();
		mSystem = NULL;
		return;
	}
#	else
	if(FALSE == FSOUND_Init(44100, 32, 0))
	{
		wxLogError(wxT("Sound Initialization Error: %s"), 
			HashString(FMOD_ErrorString(FSOUND_GetError())).cwx_str());
	}
#	endif
}

#ifdef FMOD4
SoundEngine::~SoundEngine()
{
	if(NULL != mSystem)
	{
		// release all sounds loaded
		FMOD::Sound *sound;
		FMOD::SoundGroup *soundGroup;
		int soundc;

		mSystem->getMasterSoundGroup(&soundGroup);
		soundGroup->getNumSounds(&soundc);

		for(int i = 0; i < soundc; ++i)
		{
			soundGroup->getSound(i, &sound);
			sound->release();
		}

		// release the sound system
		mSystem->close();
		mSystem->release();
	}
}
#endif

void SoundEngine::PlaySound(const wxString &sound) const
{
	//see if we even need to play a sound
	if(FALSE == CONFIG.read<wxInt32>(swConfigSectionGeneral, swPlaySounds, TRUE))
	{
		return;
	}

#	ifdef FMOD4
	if(NULL == mSystem)
	{
		return;
	}
#	endif

	wxASSERT(false == sound.IsEmpty());

	wxString path;
	path = FilePath::Sound(sound);

	//load the sound
#	ifdef FMOD4
	FMOD::Sound *fsound;
	FMOD::ChannelGroup *fgroup;
	FMOD_RESULT fresult;
	int numChannels;

	// housekeeping
	// release sounds if none are currently playing
	mSystem->getMasterChannelGroup(&fgroup);
	fgroup->getNumChannels(&numChannels);

	if(0 < numChannels)
	{
		FMOD::Channel *channel;
		bool isPlaying;

		for(int i = 0; i < numChannels; ++i)
		{
			fgroup->getChannel(i, &channel);
			channel->isPlaying(&isPlaying);

			if(true == isPlaying)
			{
				continue;
			}

			channel->getCurrentSound(&fsound);
			if(0 != fsound)
			{
				fsound->release();
			}
		}
	}

	// load the sound
	fresult = mSystem->createSound(path.ToAscii(), FMOD_SOFTWARE, 0, &fsound);
	if(FMOD_OK != fresult)
	{
		wxLogError(wxT("Can't create sound: %s, %s"), path.c_str(),
			FMOD_ErrorString(fresult));
		return;
	}
#	else
	FSOUND_SAMPLE *fsound = FSOUND_Sample_Load(FSOUND_FREE, path.ToAscii(), 
		FSOUND_NORMAL, 0, 0);

	if(NULL == fsound)
	{
		wxLogError(wxT("Sound Error: %s, %s"), path.c_str(), 
			FMOD_ErrorString(FSOUND_GetError()));
		return;
	}
#	endif

	//play the sound
#	ifdef FMOD4
	fresult = mSystem->playSound(FMOD_CHANNEL_FREE, fsound, 0, NULL);
	if(FMOD_OK != fresult)
	{
		wxLogError(wxT("Can't play sound: %s, %s"), path.c_str(),
			FMOD_ErrorString(fresult));
		return;
	}
#	else
	wxInt32 ret = FSOUND_PlaySound(FSOUND_FREE, fsound);

	if(-1 == ret)
	{
		wxLogError(wxT("Sound Error: %s, %s"), path.c_str(), 
			FMOD_ErrorString(FSOUND_GetError()));
		return;
	}
#	endif
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//


