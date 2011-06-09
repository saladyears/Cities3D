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
#include <wx/aui/aui.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "BaseFrame.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxListEvent;
class DataObject;
class wxBaseListCtrl;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxSettlersFrame
// 
// The top-level frame window.  Houses the main screen window
// when a game is not being played, and a <SettlersView> when
// a game is being played.
//
// Derived From:
//     <wxBaseFrame>
//
// Project:
//     <Cities3D>
//
// Include:
//     SettlersFrame.h
//
class wxSettlersFrame : public wxBaseFrame
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxMainFrame
	//
	// The wxMainFrame constructor.  Initializes the
	// base class.  Sets the window size based on 
	// the values in the <ConfigDatabase>.
	//
	// Parameters:
	//     parent - The parent window.  May be NULL.
	//     pos - The window position.
	//     size - The window size.
	//
	wxSettlersFrame(wxWindow *parent, const wxPoint &pos = wxDefaultPosition, 
		const wxSize &size = wxDefaultSize);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxSettlersFrame
	//
	// The wxSettlersFrame destructor.  Saves the window
	// size to the <ConfigDatabase>.
	//
	~wxSettlersFrame();

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Size
	//
	// Handles a resize event.  Checks to see if the window
	// is maximized.
	//
	// Parameters:
	//     event - The size event.
	//
	virtual void Size(wxSizeEvent &event);

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
	typedef std::pair<wxString, wxString> StringPair;
	typedef std::vector<StringPair> StringPairArray;

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: RestartEngine
	//
	// Forces the engine to reload during the next idle cycle.
	//
	void RestartEngine();

	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnStartGame
	//
	// Hides the main menu at game start and creates the game window.
	//
	// Parameters:
	//     newGame - A bool that is true if the game is a new game, and false
	//               if it is not.
	//
	void OnStartGame(bool newGame);

	//-----------------------------------------------------------------------//
	// Function: OnInitializeUI
	//
	// Initializes all UI in the game with the given <Game> data and the
	// AUI manager.
	//
	// Parameters:
	//     game - The current <Game>.
	//
	void OnInitializeUI(const GamePtr& game);

	//-----------------------------------------------------------------------//
	// Function: OnQuit
	//
	// Handles the player quitting a game.
	//
	// Parameters:
	//     window - The window the quit message is coming from.
	//
	void OnQuit(wxWindow* window);

	//-----------------------------------------------------------------------//
	// Function: OnExit
	//
	// Handles the player exiting the application.
	//
	// Parameters:
	//     window - The window the quit message is coming from.
	//
	void OnExit(wxWindow* window);

	//-----------------------------------------------------------------------//
	// Function: OnImmediateExit
	//
	// Terminates the application immediately.  This should only be called in 
	// the most dire circumstances.
	//
	void OnImmediateExit(wxInt32);

	//-----------------------------------------------------------------------//
	// Function: OnRequestUserAttention
	//
	// Handles the window requiring attention.
	//
	void OnRequestUserAttention(wxInt32);

	//-----------------------------------------------------------------------//
	// Function: OnChangeMainPane
	//
	// Handles the user selecting a new main pane window.
	//
	// Parameters:
	//     object - The <DataObject> containing the relevant pane information.
	//
	void OnChangeMainPane(const DataObject& object);

	//-----------------------------------------------------------------------//
	// Function: OnNewSidePane
	//
	// Handles a new side pane being set up.
	//
	// Parameters:
	//     object - The <DataObject> containing the relevant pane information.
	//
	void OnNewSidePane(const DataObject& object);

	//-----------------------------------------------------------------------//
	// Function: OnAddSidePane
	//
	// Handles a new pane being add to the side notebook pane.
	//
	// Parameters:
	//     object - The <DataObject> containing the relevant pane information.
	//
	void OnAddSidePane(const DataObject& object);

	//-----------------------------------------------------------------------//
	// Function: OnUncheckMenu
	//
	// Handles the user closing a pane controlled by a menu item.
	//
	// Parameters:
	//     id - The id of the menu item controlling the pane.
	//
	void OnUncheckMenu(wxInt32 id);

	//-----------------------------------------------------------------------//
	// Function: OnSaveLayout
	//
	// Handles the user saving the current layout.
	//
	void OnSaveLayout(wxInt32);

	//-----------------------------------------------------------------------//
	// Function: OnRestoreLayout
	//
	// Handles the user restoring the default layout.
	//
	void OnRestoreLayout(wxInt32);

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnMaximize
	//
	// Handles the window being maximized or restored.
	//
	// Parameters:
	//     event - The maximize event.
	//
	void OnMaximize(wxMaximizeEvent& event);

	//-----------------------------------------------------------------------//
	// Function: OnPaneClose
	//
	// Handles a pane window being closed.
	//
	// Parameters:
	//     event - The maximize event.
	//
	void OnPaneClose(wxAuiManagerEvent& event);

	//-----------------------------------------------------------------------//
	// Function: OnActivate
	//
	// Sets the window dirty flag when the frame is activated or deactivated.
	//
	void OnActivate(wxActivateEvent &);

	//-----------------------------------------------------------------------//
	// Function: OnButton
	//
	// Generic handler for main menu clicks.
	//
	// Parameters:
	//     event - The menu command event.
	//
	void OnButton(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnMenu
	//
	// Generic handler for game menu clicks.
	//
	// Parameters:
	//     event - The menu command event.
	//
	void OnMenu(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnIdle
	//
	// Handles idle events.
	//
	// Parameters:
	//     event - The idle event.
	//
	void OnIdle(wxIdleEvent& event);


	//-----------------------------------------------------------------------//
	// Function: OnClose
	//
	// Handles the close event.
	//
	// Parameters:
	//     event - The close event.
	//
	void OnClose(wxCloseEvent& event);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Int: mClickId
	//
	// The id of any button that was clicked that we need to resimulate.
	//
	wxInt32 mClickId;

	//-----------------------------------------------------------------------//
	// Bool: mWasMaximized
	//
	// Flag that is true if the window is maximized.
	//
	bool mWasMaximized;

	//-----------------------------------------------------------------------//
	// Bool: mRestartEngine
	//
	// Flag that is true if the game engine should be restarted and false
	// if not.
	//
	bool mRestartEngine;

	wxAuiManager mAuiManager;
};

//---------------------------- PROTOTYPES       -----------------------------//

