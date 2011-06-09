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
#include <wx/apptrait.h>

//---------------------------- USER INCLUDES    -----------------------------//

//---------------------------- DEFINES          -----------------------------//
#define wxTheCitiesOnlineApp static_cast<wxCitiesOnlineApp*>(wxTheApp)

//---------------------------- TYPEDEFS         -----------------------------//
class wxCitiesOnlineTaskbarIcon;
class wxSingleInstanceChecker;
class wxCitiesOnlineServer;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxCitiesOnlineApp
// 
// The main launcher application.
//
// Derived From:
//     wxApp
//
// Project:
//     <CitiesOnline>
//
// Include:
//     app.h
//
class wxCitiesOnlineApp: public wxApp
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: InGame
	//
	// Sets the flag that stores whether the player is currently in an online
	// game or not.
	//
	// Parameters:
	//   inGame - True if the player is in a game and false if not.
	//
	void InGame(bool inGame)
	{
		mInGame = inGame;
	}

	//-----------------------------------------------------------------------//
	// Function: InGame
	//
	// Returns whether the player is an online game or not.
	//
	// Returns:
	//   True if the player is in a game and false if not.
	//
	bool InGame() const
	{
		return mInGame;
	}

	//-----------------------------------------------------------------------//
	// Function: PostUrl
	//
	// Posts the given url to the icon window so that it gets processed in the
	// main thread.
	//
	// Parameters:
	//     url - The url to process.
	//
	void PostUrl(const wxString& url);

	//-----------------------------------------------------------------------//
	// Function: ProcessUrl
	//
	// Processes the url once notified in the main thread.
	//
	void ProcessUrl();

	//-----------------------------------------------------------------------//
	// Function: ProcessString
	//
	// Processes the command line returned from the launcher.
	//
	// Parameters:
	//     str - The string to process.
	//
	void ProcessString(const wxString& str);

	//-----------------------------------------------------------------------//
	// Function: CommandUrl
	//
	// Returns the command url used to pass notifications to the site.
	//
	// Returns:
	//     The command url used to pass notifications to the site.
	//
	const wxString& CommandUrl() const
	{
		return mCommandUrl;
	}

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	class wxCitiesOnlineAppTraits : public wxGUIAppTraits
	{
	public:
		virtual wxMessageOutput* CreateMessageOutput()
		{
			return new wxMessageOutputLog;
		}
	};

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnInit
	//
	// Initializes the application.
	//
	// Returns:
	//     A bool indicating success initialization of the
	//     application.  If false, the application is terminated.
	//
    virtual bool OnInit();

	//-----------------------------------------------------------------------//
	// Function: OnExit
	//
	// Exits the application.
	// 
	// Returns:
	//     The value returned by wxApp::OnExit()
	//
	virtual int OnExit();

	//-----------------------------------------------------------------------//
	// Function: CreateTraits
	//
	// Creates the specific application traits for the CitiesOnline app.
	// 
	// Returns:
	//     A pointer to the application traits.
	//
	virtual wxAppTraits* CreateTraits()
	{
		return new wxCitiesOnlineAppTraits;
	}

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// String: mCommandUrl
	//
	// The url used to send commands to the site.
	//
	wxString mCommandUrl;

	//-----------------------------------------------------------------------//
	// String: mUrl
	//
	// The url posted from the server thread.
	//
	wxString mUrl;

	//-----------------------------------------------------------------------//
	// Object: mpTaskbarIcon
	//
	// The taskbar tray icon.
	//
	boost::scoped_ptr<wxCitiesOnlineTaskbarIcon> mpTaskbarIcon;

	//-----------------------------------------------------------------------//
	// Object: mpInstanceChecker
	//
	// The single instance checker.
	//
	boost::scoped_ptr<wxSingleInstanceChecker> mpInstanceChecker;

	//-----------------------------------------------------------------------//
	// Object: mpServer
	//
	// The socket server that listens for command lines and host checks.
	//
	boost::scoped_ptr<wxCitiesOnlineServer> mpCitiesOnlineServer;

    //-----------------------------------------------------------------------//
	// Object: mLocale
	//
	// The locale for the application, which determines which language to use.
	//
	boost::scoped_ptr<wxLocale> mpLocale;

	//-----------------------------------------------------------------------//
	// Bool: mInGame
	//
	// True if the player is in a game and false if not.
	//
	bool mInGame;
};

//---------------------------- PROTOTYPES       -----------------------------//

