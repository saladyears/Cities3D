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
#include <wx/snglinst.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "CitiesOnlineApp.h"
#include "Synchronizer.h"
#include "TimerManager.h"
#include "Launcher.h"
#include "encryption/Encryption.h"
#include "synchronization/Synchronization.h"
#include "IPluginEngine.h"
#include "sha.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	// Half a second seems reasonable.
	const size_t SYNCHRONIZATION_RESOLUTION = 500;
}

//---------------------------- PUBLIC           -----------------------------//
void Synchronizer::Start(const std::string& playerName)
{
	mPlayerName = playerName;

	Synchronization::CreateSynchronizationPathToCitiesOnline(
		mSynchronizationFile);

	// Add the timer.  This puts it in the main thread so we don't
	// have to deal with threading issues.
	TIME.AddPeriodicFunction(&Synchronizer::Synchronize, this,
		SYNCHRONIZATION_RESOLUTION);
}

void Synchronizer::Stop()
{
	// Cleanup.
	TIME.RemovePeriodicFunction(&Synchronizer::Synchronize, this,
		SYNCHRONIZATION_RESOLUTION);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void Synchronizer::Synchronize()
{
	bool inGame = wxTheCitiesOnlineApp->InGame();

	// If the synchronization file exists, it was put there by 
	// CitiesOnline.  Open it, read the contents, delete it, then
	// parse the contents.
	if(true == wxFile::Exists(mSynchronizationFile.c_str()))
	{
		boost::scoped_ptr<char> buffer;
		wxFileOffset length = 0;

		{
			wxFile file(mSynchronizationFile.c_str());
			length = file.Length();

			// Read the contents.
			buffer.reset(new char[length]);
			file.Read(buffer.get(), length);
		}

		::wxRemoveFile(mSynchronizationFile);

		// The file is structured such that the first 4 bytes of every block 
		// give the length of the block.  It is possible that multiple 
		// commands may be present.  Cities Online may have been shut down
		// while Cities3D was issuing commands so we need to deal with it
		// gracefully.
		StdStringArray commands;
		const char* buf = buffer.get();
		const char* p = buf;

		while(p < (buf + length))
		{
			const size_t commandLength = 
				*reinterpret_cast<const wxUint32*>(p);
			p += sizeof(size_t);

			std::string encryptedCommand(p, commandLength);
			p += commandLength;

			byte sha[CryptoPP::SHA::DIGESTSIZE];
			Encryption::HashSha1(mPlayerName, sha);

			std::string dt;
			Encryption::Decrypt(encryptedCommand, sha, 
				CryptoPP::SHA::DIGESTSIZE, dt);

            std::string decryptedText = dt.c_str();

			// Handle in game/not in game notification.
			switch(decryptedText[0])
			{
			case '+':
				inGame = true;
				break;
			case '-':
				inGame = false;
				break;
			case '?':
				// Store for processing.
				commands.push_back(decryptedText);
				break;
			default:
				break;
			}
		}

		// Send the commands to the site.
		Launcher::get().Command(commands);
	}

	// If we ever get to a point where Cities3D is not open, then we are 
	// certainly not in a game.
	{
		wxSingleInstanceChecker instanceChecker(swCities3D);

		if(false == instanceChecker.IsAnotherRunning())
		{
			inGame = false;
		}
	}

	if(inGame != wxTheCitiesOnlineApp->InGame())
	{
		wxTheCitiesOnlineApp->InGame(inGame);
	}
}
