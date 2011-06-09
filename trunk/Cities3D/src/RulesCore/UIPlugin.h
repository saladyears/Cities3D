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
#include "IUIPlugin.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: UIPlugin
// 
// The base class for plugin UI items.
//
// Derived From:
//     <IUIPlugin>
//
// Project:
//     <RulesCore>
//
// Include:
//     UIPlugin.h
//
class RULES_EXPORT_SPEC UIPlugin : public IUIPlugin
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: SetMainFrame
	//
	// Sets the main frame window.
	//
	// Parameters:
	//     window - The main frame.
	//
	virtual void SetMainFrame(wxWindow* window)
	{
		mMainFrame = window;
	}

	//-----------------------------------------------------------------------//
	// Function: ProvidesUI
	//
	// Returns whether this plugin creates any game UI.
	//
	// Returns:
	//     A bool that is true if the plugin creates game UI and false if not.
	//
	virtual bool ProvidesUI() const
	{
		return false;
	}

	//-----------------------------------------------------------------------//
	// Function: CreateUI
	//
	// Creates any UI for the game this plugin provides.
	//
	// Parameters:
	//     game - The current <Game>.
	//     manager - The AUI manager.
	//
	virtual void CreateUI(const GamePtr&, wxAuiManager&)
	{
		wxASSERT(false == ProvidesUI());
	}

	//-----------------------------------------------------------------------//
	// Function: SetGLCanvas
	//
	// Sets the GL canvas so created UI may have access to GL-rendered bitmaps.
	//
	// Parameters:
	//     canvas - The GL canvas.
	//
	virtual void SetGLCanvas(wxSettlersGLCanvas* canvas)
	{
		mGLCanvas = canvas;
	}

	//-----------------------------------------------------------------------//
	// Function: HandleMainMenuClick
	//
	// Tells the UI to handle an item click with the given key.
	//
	// Parameters:
	//     key - The key of the item click.
	//
	virtual void HandleMainMenuClick(const wxString&)
	{
		// Default to nothing.
	}

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
	virtual void HandleGameMenuClick(const wxString&, wxAuiManager&, 
		wxMenuItem*)
	{
		// Default to nothing.
	}

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
	virtual QuitHandler GetQuitHandler(wxInt32& o_priority)
	{
		o_priority = 0;

		// Default to no handler.
		return QuitHandler();
	}

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: UIPlugin
	//
	// The UIPlugin constructor.
	//
	UIPlugin();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: GetMainFrame
	//
	// Returns the pointer to the main application frame.
	//
	// Returns:
	//     A pointer to the main frame.  Guaranteed to not be NULL.
	//
	wxWindow* GetMainFrame() const
	{
		wxASSERT(NULL != mMainFrame);
		return mMainFrame;
	}

	//-----------------------------------------------------------------------//
	// Function: GetGameWindow
	//
	// Returns the pointer to the game window.
	//
	// Returns:
	//     A pointer to the game window.  Guaranteed to not be NULL.
	//
	wxSettlersGLCanvas* GetGLCanvas() const
	{
		wxASSERT(NULL != mGLCanvas);
		return mGLCanvas;
	}

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Pointer: mMainFrame
	// 
	// The main application frame.
	// 
	wxWindow* mMainFrame;

	//-----------------------------------------------------------------------//
	// Pointer: mGameWindow
	// 
	// The main game window that UI can access to request OpenGL-rendered
	// bitmaps.
	// 
	wxSettlersGLCanvas* mGLCanvas;
};

//---------------------------- PROTOTYPES       -----------------------------//
