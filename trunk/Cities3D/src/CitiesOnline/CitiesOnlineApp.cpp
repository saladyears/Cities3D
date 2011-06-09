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
#include <wx/cmdline.h>
#include <dll.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "CitiesOnlineApp.h"
#include "CitiesOnlineTaskbarIcon.h"
#include "CitiesOnlineServer.h"
#include "Launcher.h"
#include "Process.h"
#include "Synchronizer.h"
#include "PortForwardManager.h"
#include "ConfigDatabase.h"
#include "FilePath.h"
#include "synchronization/Synchronization.h"
#include "encryption/Encryption.h"
#include "network/Network.h"

#ifdef __WXGTK__
#include "FilePath.h"
#endif

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_APP(wxCitiesOnlineApp)

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

	// This is a little bit of trickery to avoid having command line parameters
	// show up in plain text in the executable string table.
	const char sParam1[] = 
	{
		'h' - 32,
		'o' - 16,
		's' - 8,
		't' - 4,
		'c' - 2,
		'h' - 1,
		'e' + 1,
		'c' + 2,
		'k' + 4,
		'\0'
	};

	const char sParam3[] = 
	{
		'p' - 32,
		'l' - 16,
		'a' - 8,
		'y' - 4,
		'e' - 2,
		'r' - 1,
		'\0'
	};

	// Note: this must match the bytes used to decrypt in CitiesOnlineRules
	// exactly.
	const char sParam5[] = 
	{
		'p' - 32,
		'l' - 16,
		'e' - 8,
		'@' - 4,
		'~' - 2,
		's' - 1,
		'E' + 1,
		'l' + 2,
		'?' + 4,
		'K' + 8,
		'\0'
	};

	const char sParam6[] =
	{
		'r' - 32,
		'e' - 16,
		'm' - 8,
		'a' - 4,
		'p' - 2,
		'\0'
	};

	// Intentionally misnamed.
	void MessageLoop(const char* in, std::string& out)
	{
		out = in;
		size_t size = out.length();
		wxInt32 delta = 32;
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

#ifdef _WIN32
#ifdef _DEBUG
	const wxString sCitiesExe = swCities3DDExe;
#else
	const wxString sCitiesExe = swCities3DExe;
#endif	// _DEBUG
#else // _WIN32
	const wxString sCitiesExe = swCities3D;
#endif // __linux__
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
void wxCitiesOnlineApp::PostUrl(const wxString& url)
{
	mUrl = url;

	// This is convoluted, but the icon taskbar is the only thing processing
	// messages in the main thread on the application, so it has to get the
	// notification.  wxWidgets chokes if you run a modal dialog in a thread
	// other than the main thread.
	wxCommandEvent event(wxEVT_COMMAND_MENU_SELECTED, 
		mpTaskbarIcon->GetUrlMenuId());

	mpTaskbarIcon->AddPendingEvent(event);
}

void wxCitiesOnlineApp::ProcessUrl()
{
	bool ret = Launcher::get().Launch(mUrl);
	mUrl.Clear();

	// This would be pretty rare, but they might have CitiesOnline open when
	// the server version gets updated.
	if(false == ret)
	{
		ExitMainLoop();
	}
}

void wxCitiesOnlineApp::ProcessString(const wxString& str)
{
	wxString commandLine = str;

	wxCmdLineParser parser(commandLine);

	// Get all of the parameters.
	std::string p1;
	MessageLoop(sParam1, p1);
	std::string p3;
	MessageLoop(sParam3, p3);
	std::string p6;
	MessageLoop(sParam6, p6);

	wxString param1 = wxString(p1.c_str(), wxConvLibc);
	wxString param3 = wxString(p3.c_str(), wxConvLibc);
	wxString param6 = wxString(p6.c_str(), wxConvLibc);
	
	// Add our options.
	parser.AddOption(param1, wxEmptyString, wxEmptyString, 
		wxCMD_LINE_VAL_STRING, wxCMD_LINE_NEEDS_SEPARATOR);
	parser.AddOption(param3, wxEmptyString, wxEmptyString, 
		wxCMD_LINE_VAL_STRING, wxCMD_LINE_NEEDS_SEPARATOR);
	parser.AddOption(param6, wxEmptyString, wxEmptyString, 
		wxCMD_LINE_VAL_STRING, wxCMD_LINE_NEEDS_SEPARATOR);
	
	// Wrap this in NULL log output so no information is leaked.
	{
		wxLogNull log;
		parser.Parse(false);
	}

	// Store the command URL for future commands.
	wxString command;
	if(true == parser.Found(param6, &command))
	{
		mCommandUrl = command;

		// Remove it from the string so it doesn't affect the client's parsing
		// of the string.
		size_t len = command.Length();
		wxInt32 index = commandLine.Find(command.c_str());
		if(-1 != index)
		{
			commandLine = commandLine.Right(commandLine.Length() - len - index - 1);
		}
	}

	// If we got the options, fire up the server.  Otherwise, we need to
	// launch the client.
	wxString player, session;
	if( (true == parser.Found(param1, &session)) &&
		(true == parser.Found(param3, &player)))
	{
		// First, get the user's name and id.
		wxString name;
		wxInt32 id;
		if(true == Network::SplitNameAndId(player, name, id))
		{
			long port = PortForwardManager::get().GetPort();

			// Launch the host check.
			mpCitiesOnlineServer->Hostcheck(port);

			StdStringArray commands;

			// Send the command to the site.
			wxString cmd = wxString::Format(swC1.c_str(), param1.c_str(), 
				name.c_str(), session.c_str(), port);

			commands.push_back(std::string(cmd.char_str()));
			
			Launcher::get().Command(commands);
		}
	}
	else
	{
		// Write the synchronization file.
		wxString synchronizationPath;
		Synchronization::CreateSynchronizationPathToCities3D(synchronizationPath);

		// Hash the password.
		std::string param;
		MessageLoop(sParam5, param);

		byte sha[CryptoPP::SHA::DIGESTSIZE];
		Encryption::HashSha1(param, sha);
		
		// Encrypt the commands.
		std::string input = (char*)commandLine.char_str();
		std::string output;
		Encryption::Encrypt(input, sha, CryptoPP::SHA::DIGESTSIZE, output);
		
		// Write the file.
		wxFile file(synchronizationPath.c_str(), wxFile::write);
		file.Write(output.c_str(), output.length());

		// Launch the executable, if needed.
		wxSingleInstanceChecker instanceChecker(swCities3D);
		if(false == instanceChecker.IsAnotherRunning())
		{
			// Launch.
			wxFileName fname(wxStandardPaths::Get().GetExecutablePath());
			fname.SetFullName(sCitiesExe);
			::wxExecute(fname.GetFullPath());
		}
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
bool wxCitiesOnlineApp::OnInit()
{
    // Set the locale.
    wxString str = CONFIG.read<wxString>(swConfigSectionGeneral, swLanguage, 
		swEmpty);

	if(false == str.IsEmpty())
	{
		// Find the language info for this string and set it as the locale.
		const wxLanguageInfo *info = wxLocale::FindLanguageInfo(str);
		wxASSERT(NULL != info);

		if(NULL != info)
		{
			mpLocale.reset(new wxLocale(info->Language));

			// Immediately add the correct lookup paths.
			wxArrayString localePaths = FilePath::EnumLocaleDirs(info->CanonicalName);
			for(size_t i = 0; i < localePaths.GetCount(); ++i)
				mpLocale->AddCatalogLookupPathPrefix(localePaths[i]);

			// Finally, add the Cities3D domain, which covers CitiesOnline.
			mpLocale->AddCatalog(swCities3D);
		}
	}

	// First, a sanity check.  CitiesOnline is only ever validly launched with
	// a single encrypted command line argument.  Therefore, argc must always
	// be 2.  If it is not, someone is trying to run it off disk and not from
	// the site.
	if(2 != argc)
	{
        static const wxString stCitiesOnlineMayOnlyBeLaunched = _("Cities Online may only be launched from the site.");
		wxMessageBox(stCitiesOnlineMayOnlyBeLaunched);

		return false;
	}

	// We are not in a game to start.
	mInGame = false;

	bool ret = true;

#ifdef __WXGTK__
	const wxString nat_theme_path = FilePath::Skin(swGtkrc);
	if(false == (SetNativeTheme(nat_theme_path)))
	{
		wxLogDebug(wxT("Can't load gtkrc file: %s"), nat_theme_path);
	}
#endif // __WXGTK__

	// Test for an existing instance.  If we have it, tell it what command line
	// we are being passed and abort.
	mpInstanceChecker.reset(new wxSingleInstanceChecker(swCitiesOnlineNoSpaces));
	if(true == mpInstanceChecker->IsAnotherRunning())
	{
		// It's running.  Connect to it and tell it what the command line is.
		bool sent = wxCitiesOnlineServer::SendMessage(argv[1]);

		// If we successfully sent, we're done here, exit this instance.
		if(true == sent)
		{
			ret = false;
		}
		// If we failed to send, ruh roh.  Something is messed up.  Kill the
		// other instance and handle the message ourselves.
		else
		{
			Process::TerminateOtherProcess(argv[0]);
		}
	}

	// Handle the command line if required.
	if(true == ret)
	{
		// Start up the PortForwardManager to give it as much time as possible 
		// to find or create port mappings.
		PortForwardManager::get().Initialize();

		// This is the first instance so set the taskbar icon and start up
		// the server and handle the command line.
		mpTaskbarIcon.reset(new wxCitiesOnlineTaskbarIcon);

		// This is required for sockets running in different threads.
		wxSocketBase::Initialize();

		// Start up the server.
		mpCitiesOnlineServer.reset(new wxCitiesOnlineServer);
	
		ret = Launcher::get().Launch(argv[1]);

		if(false == ret)
		{
			mpCitiesOnlineServer.reset();
			mpTaskbarIcon.reset();
		}
	}

	// Start up the synchronizer between Cities3D and CitiesOnline.
	std::string param;
	MessageLoop(sParam5, param);
	Synchronizer::get().Start(param);
	
    return ret;
}

int wxCitiesOnlineApp::OnExit()
{
	PortForwardManager::get().Shutdown();

	Synchronizer::get().Stop();

	mpTaskbarIcon.reset();
	mpCitiesOnlineServer.reset();

	return wxApp::OnExit();
}

