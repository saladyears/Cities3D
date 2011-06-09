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
#include <dll.h>
#include <wx/snglinst.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "synchronization/Synchronization.h"
#include "encryption/Encryption.h"
#include "GameEngine.h"
#include "TimerManager.h"
#include "Controller.h"
#include "CommandProcesser.h"
#include "ITransientCommand.h"
#include "BaseMessageBox.h"
#include "IPluginEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	// These are used so we can set a breakpoint in CryptoPP allocations.
	void* crypto_new(size_t size)
	{
		return malloc(size);
	}

	void crypto_delete(void* mem)
	{
		free(mem);
	}

	// Half a second seems reasonable.
	const size_t SYNCHRONIZATION_RESOLUTION = 500;

#ifdef _WIN32
#	ifdef _DEBUG
		const wxString sCitiesExe = swCities3DDExe;
#	else
		const wxString sCitiesExe = swCities3DExe;
#	endif	// _DEBUG
#else // _WIN32
	const wxString sCitiesExe = swCities3D;
#endif

	// Note: this must match the bytes used to encrypt in CitiesOnline
	// exactly.
	const char param[] = 
	{
		'p' - 16,
		'l' - 8,
		'e' - 4,
		'@' - 2,
		'~' - 1,
		's' + 1,
		'E' + 2,
		'l' + 4,
		'?' + 8,
		'K' + 16,
		'\0'
	};

	// Intentionally misnamed.
	void SanityCheck(const char* in, std::string& out)
	{
		out = in;
		size_t size = out.length();
		wxInt32 delta = 16;
		bool right = true;
		for(size_t i = 0; i < size; ++i)
		{
			out[i] += delta;

			if(true == right)
			{
				delta >>= 1;
			}
			else
			{
				delta <<= 1;
			}

			if(0 == delta)
			{
				right = false;
				delta = -1;
			}
		}
	}

	class wxSynchronizer
	{
	public:
		wxSynchronizer()
			: mInStartup(false)
		{
			if(false == GameEngine::get().InStartup())
			{
				Synchronization::CreateSynchronizationPathToCities3D(
					mSynchronizationFile);

				// Add the timer.  This puts it in the main thread so we don't
				// have to deal with threading issues.
				TIME.AddPeriodicFunction(&wxSynchronizer::Synchronize, this,
					SYNCHRONIZATION_RESOLUTION);

				// Add the listener to intercept startup.
				Controller::get().AddReceiver(shEventStartup, 
					&wxSynchronizer::OnStartup, this);
				Controller::get().AddReceiver(shEventTransientCommand,
					&wxSynchronizer::OnTransientCommand, this);
				Controller::get().AddReceiver(shEventSynchronize,
					&wxSynchronizer::OnSynchronize, this);
			}
		}

		~wxSynchronizer()
		{
			if(false == GameEngine::get().InStartup())
			{
				// Cleanup.
				TIME.RemovePeriodicFunction(&wxSynchronizer::Synchronize, this,
					SYNCHRONIZATION_RESOLUTION);

				Controller::get().RemoveReceiver(shEventStartup, 
					&wxSynchronizer::OnStartup, this);
				Controller::get().RemoveReceiver(shEventTransientCommand,
					&wxSynchronizer::OnTransientCommand, this);
				Controller::get().RemoveReceiver(shEventSynchronize,
					&wxSynchronizer::OnSynchronize, this);
			}
		}

	private:
		void Synchronize()
		{
			// Handle transient commands.
			if(mTransientCommand)
			{
				// Make a copy of the command for thread safety.
				TransientCommandPtr command = mTransientCommand;

				// Kill the stored copy so we don't hit it again.
				mTransientCommand.reset();

				// Execute the copy.
				command->Execute();
			}

			// If the synchronization file exists, it was put there by 
			// CitiesOnline.  Open it, read the contents, delete it, then
			// parse the contents.
			if(true == wxFile::Exists(mSynchronizationFile.c_str()))
			{
				// This is extraordinarily cheap, but if we're already in a 
				// game, (i.e. hotseat, or non-Cities Online network), then
				// just nuke the game entirely and relaunch with the
				// synchronization file intact.  It will get picked up on
				// startup and immediately fire from a clean start.
				IPluginEngine::StringPairArray rulesets;
				PLUGIN.GetLoadedRuleSets(rulesets);

				// If we have more than the core two (CoreRules:Core and
				// CitiesOnlineRules:CitiesOnline) loaded, bail.
				if(2 < rulesets.size())
				{
					wxFileName fname(wxStandardPaths::Get().GetExecutablePath());
					fname.SetFullName(sCitiesExe);
					::wxExecute(fname.GetFullPath());
					Controller::get().Transmit(shEventImmediateExit, 0);
					return;
				}

				std::string fileContents;

				{
					wxFile file(mSynchronizationFile.c_str());
					wxFileOffset length = file.Length();

					// Read the contents.
					boost::scoped_ptr<char> buffer(new char[length]);
					file.Read(buffer.get(), length);

					fileContents = std::string(buffer.get(), length);
				}

				// Delete the file.
				::wxRemoveFile(mSynchronizationFile);

				// Only do something if we got something different than last
				// time.
				if(mLastFileContents != fileContents)
				{
					mLastFileContents = fileContents;

					// Decrypt the contents.
					std::string param1;
					SanityCheck(param, param1);

					byte sha[CryptoPP::SHA::DIGESTSIZE];
					Encryption::HashSha1(param1, sha);

					std::string decryptedText;
					Encryption::Decrypt(fileContents, sha, 
						CryptoPP::SHA::DIGESTSIZE, decryptedText);

					// Convert to wxString and process.
					wxString commands = wxString(decryptedText.c_str(), 
						wxConvLibc, decryptedText.length()).Trim();

					CommandProcesser::get().ProcessCommand(commands, mInStartup);
				}
			}
		}

		// Once we get the startup notification, immediately handle 
		// synchronization.  We may have been launched from CitiesOnline.
		void OnStartup(int)
		{
			mInStartup = true;
			Synchronize();
			mInStartup = false;
		}

		void OnTransientCommand(const TransientCommandPtr& command)
		{
			// Assume we do not have more than one transient command at a time.
			wxASSERT(!mTransientCommand);

			mTransientCommand = command;
		}

		void OnSynchronize(const HashString& command)
		{
			// Make sure we still have Cities Online running.
			if(false == CitiesOnlineCheck())
			{
				return;
			}			

			// Synchronize the given command to Cities Online for sending to
			// the site.

			// Encrypt it for passage between applications.
			std::string param1;
			SanityCheck(param, param1);

			byte sha[CryptoPP::SHA::DIGESTSIZE];
			Encryption::HashSha1(param1, sha);

			std::string encryptedText;
			Encryption::Encrypt(command, sha, CryptoPP::SHA::DIGESTSIZE,
				encryptedText);

			wxString path;
			Synchronization::CreateSynchronizationPathToCitiesOnline(path);

			// Open the file for append.
			wxFile file(path.c_str(), wxFile::write_append);
			
			if(true == file.IsOpened())
			{
				// Write 4 bytes with the length of our encrypted command
				// and then the command itself.
				size_t length = encryptedText.size();
				file.Write(&length, sizeof(size_t));
				file.Write(encryptedText.c_str(), length);
				file.Close();
			}
			else
			{
				// TODO: If we can't open the file, something is terribly
				// wrong.  Probably need to quit here.
			}
		}

		bool CitiesOnlineCheck()
		{
			bool ret = true;

			// Check for Cities Online here, shut down if doesn't exist.
			wxSingleInstanceChecker instance(swCitiesOnlineNoSpaces);

			if(false == instance.IsAnotherRunning())
			{
				// Cities Online was shut down during the game, announce 
				// what happened and exit the game.
				wxBaseMessageBox dialog(NULL, swCitiesOnline, 
                    stCitiesOnlineWasShutDown);
				dialog.ShowModal();

				Controller::get().Transmit(shEventImmediateExit, 0);
				ret = false;
			}

			return ret;
		}

		TransientCommandPtr mTransientCommand;
		wxString mSynchronizationFile;
		std::string mLastFileContents;
		bool mInStartup;
	};

	wxSynchronizer sSynchronizer;
}

// This is required so crypto++ does not use its own goofy new/delete which
// hoses over wxWidgets.
#ifdef __WIN32__
extern "C" __declspec(dllexport) void __cdecl GetNewAndDeleteForCryptoPP(
	CryptoPP::PNew &pNew, CryptoPP::PDelete &pDelete)
{
	pNew = &crypto_new;
	pDelete = &crypto_delete;
} 
#endif	// __WIN32__

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
