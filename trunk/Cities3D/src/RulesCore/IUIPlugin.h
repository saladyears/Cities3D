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
#include <boost/function.hpp>

//---------------------------- USER INCLUDES    -----------------------------//
#include "RulesExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxAuiManager;
class wxSettlersGLCanvas;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: IUIPlugin
// 
// The interface for plugin UI items.  Plugins may provide their own UI for the
// rules they implement.  To do so, they must derive an object from IUIPlugin
// and register it with their <RuleSet>.
//
// Project:
//     <RulesCore>
//
// Include:
//     IUIPlugin.h
//
class RULES_EXPORT_SPEC IUIPlugin
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	typedef boost::function1<bool, wxWindow*> QuitHandler;

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~IUIPlugin
	// 
	// The IUIPlugin destructor.  It is pure virtual because
	// this class must be derived from in order to be used.
	// 
	virtual ~IUIPlugin()=0;

	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: SetMainFrame
	//
	// Sets the main frame window.
	//
	// Parameters:
	//     window - The main frame.
	//
	virtual void SetMainFrame(wxWindow* window)=0;

	//-----------------------------------------------------------------------//
	// Function: ProvidesUI
	//
	// Returns whether this plugin creates any game UI.
	//
	// Returns:
	//     A bool that is true if the plugin creates game UI and false if not.
	//
	virtual bool ProvidesUI() const=0;

	//-----------------------------------------------------------------------//
	// Function: CreateUI
	//
	// Creates any UI for the game this plugin provides.
	//
	// Parameters:
	//     game - The current <Game>.
	//     manager - The AUI manager.
	//
	virtual void CreateUI(const GamePtr& game, wxAuiManager& manager)=0;

	//-----------------------------------------------------------------------//
	// Function: SetGLCanvas
	//
	// Sets the GL canvas so created UI may have access to GL-rendered bitmaps.
	//
	// Parameters:
	//     canvas - The GL canvas.
	//
	virtual void SetGLCanvas(wxSettlersGLCanvas* window)=0;

	//-----------------------------------------------------------------------//
	// Function: HandleMainMenuClick
	//
	// Tells the UI to handle an item click with the given key.
	//
	// Parameters:
	//     key - The key of the item click.
	//
	virtual void HandleMainMenuClick(const wxString& key)=0;

	//-----------------------------------------------------------------------//
	// Function: HandleGameMenuClick
	//
	// Tells the UI to handle a game menu click with the given key.
	//
	// Parameters:
	//     key - The key of the item click.
	//     manager - The window manager.
	//     item - The menu item clicked.
	//
	virtual void HandleGameMenuClick(const wxString& key, 
		wxAuiManager& manager, wxMenuItem* item)=0;

	//-----------------------------------------------------------------------//
	// Function: GetQuitHandler
	//
	// Returns the quit handler function provided by this UI, if any.
	//
	// Parameters:
	//     o_priority - The priority of this quit handler.  The quit handler
	//                  with the highest priority is called.
	// 
	// Returns:
	//     The quit handler function provided by this UI, if any.
	//
	virtual QuitHandler GetQuitHandler(wxInt32& o_priority)=0;
};

//---------------------------- PROTOTYPES       -----------------------------//
inline IUIPlugin::~IUIPlugin() {}
