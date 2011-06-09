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

//---------------------------- TYPEDEFS         -----------------------------//
class wxSettlersFrame;
class wxSingleInstanceChecker;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxCities3DApp
// 
// The main application class.  Part of the wxWidgets
// application infrastructure.  Creates and maintains the
// <wxSettlersFrame>, the top level frame for the main
// application window.  Switches the menus when a <wxSettlersView>
// game window is open and when it closes.  Handles
// starting and stopping the <GameEngine>.
//
// Derived From:
//     wxApp (see <http://www.wxwindows.org/manuals/2.4.2/wx30.htm>)
//
// Project:
//     <Cities3D>
//
// Include:
//     app.h
//
class wxCities3DApp: public wxApp
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnInit
	//
	// Initializes the application.  Creates the
	// <wxSettlersFrame>.  Sets the application icon.  Centers
	// the window on screen and creates the menu bar.
	// Calls ShowAppMenu to set the application menu and
	// then starts up the <GameEngine>. Runs the 
	// <wxSplashDialog> and proceeds from the results.
	//
	// Returns:
	//     A bool indicating success initialization of the
	//     application.  If false, the application is terminated.
	//
    virtual bool OnInit();

	//-----------------------------------------------------------------------//
	// Function: OnExit
	//
	// Exits the application.  Stops the <GameEngine>.
	// 
	// Returns:
	//     The value returned by wxApp::OnExit()
	//
	virtual int OnExit();

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Macro: DECLARE_EVENT_TABLE
	//
	// The wxWidgets event table declaration.
	//
	DECLARE_EVENT_TABLE()

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	class wxCities3DAppTraits : public wxGUIAppTraits
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
	// Function: CreateTraits
	//
	// Creates the specific application traits for the CitiesOnline app.
	// 
	// Returns:
	//     A pointer to the application traits.
	//
	virtual wxAppTraits* CreateTraits()
	{
		return new wxCities3DAppTraits;
	}

	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnResetLocale
	//
	// Sets the language locale of the application.
	// 
	void OnResetLocale(wxInt32 = 0);

	//-----------------------------------------------------------------------//
	// Function: OnAddCatalog
	//
	// Adds the given catalog to the current locale.
	//
	// Parameters:
	//     str - The catalog to add.
	//
	void OnAddCatalog(const wxString &str);

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnKeyDown
	//
	// Handles keys being pressed.
	//
	// Parameters:
	//     event - The key event.
	//
	void OnKeyDown(wxKeyEvent &event);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Pointer: mFrame
	//
	// The <wxSettlersFrame>, the top-level frame in the
	// application.
	//
	wxSettlersFrame *mFrame;

	//-----------------------------------------------------------------------//
	// Object: mLocale
	//
	// The locale for the application, which determines which language to use.
	//
	boost::scoped_ptr<wxLocale> mpLocale;

	//-----------------------------------------------------------------------//
	// Object: mpInstanceChecker;
	//
	// The single instance checker.
	//
	boost::scoped_ptr<wxSingleInstanceChecker> mpInstanceChecker;
};

//---------------------------- PROTOTYPES       -----------------------------//

