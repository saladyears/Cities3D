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
#include <map>

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxAuiManager;
class wxSettlersGLCanvas;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: IUIEngine
// 
// The interface of the UI engine.  The IUIEngine is responsible for keeping
// all <IUIPlugins> available while rulesets are loaded.
//
// Project:
//     <RulesCore>
//
// Include:
//     IUIEngine.h
//
class RULES_EXPORT_SPEC IUIEngine
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	struct MenuItem
	{
		bool operator==(const MenuItem& rhs) const
		{
			return key == rhs.key;
		}

		size_t index;
		wxString plugin;
		wxString ruleset;
		wxString text;
		HashString image;
		wxString key;
		wxItemKind menutype;
	};

	typedef std::map<wxInt32, MenuItem> MenuPriorityMap;
	typedef std::map<wxString, MenuPriorityMap> MenuItemMap;

	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: SetMainFrame
	//
	// Sets the main application frame for use by all plugins.
	//
	// Parameters:
	//     window - The main frame.
	//
	virtual void SetMainFrame(wxWindow* window)=0;

	//-----------------------------------------------------------------------//
	// Function: CreateUI
	//
	// Creates all UI for the game.
	//
	// Parameters:
	//     game - The current <Game>.
	//     manager - The AUI manager.
	//
	virtual void CreateUI(const GamePtr& game, wxAuiManager& manager)=0;

	//-----------------------------------------------------------------------//
	// Function: ProvidesUI
	//
	// Returns whether the given ruleset provides any game UI.
	//
	// Returns:
	//     A bool that is true if the given ruleset provides game UI and false
	//     if not.
	//
	virtual bool ProvidesUI(const wxString& plugin, const wxString& ruleset)
		const=0;

	//-----------------------------------------------------------------------//
	// Function: RegisterUI
	//
	// Registers a UI with the engine.
	//
	// Parameters:
	//     ui - The UI.
	//
	virtual void RegisterUI(wxWindow* ui)=0;

	//-----------------------------------------------------------------------//
	// Function: UnregisterUI
	//
	// Unregisters a UI with the engine.
	//
	// Parameters:
	//     ui - The UI.
	//
	virtual void UnregisterUI(wxWindow* ui)=0;

	//-----------------------------------------------------------------------//
	// Function: HasRegisteredUI
	//
	// Returns whether there are any UI registered with the engine.
	//
	// Returns:
	//     A bool that is true if there are UI registered and false if not.
	//
	virtual bool HasRegisteredUI() const=0;

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
	// Function: CanQuit
	//
	// Tests to see if the user is allowed to quit the game.
	//
	// Parameters:
	//     window - The window sending the quit message.
	//
	virtual bool CanQuit(wxWindow* window)=0;

	//-----------------------------------------------------------------------//
	// Function: HandleMainMenuClick
	//
	// Handles the item click by loading the ruleset, if needed, then calling
	// the plugin click handler.
	//
	// Parameters:
	//     index - The index of the item being clicked.
	//
	virtual void HandleMainMenuClick(size_t index)=0;

	//-----------------------------------------------------------------------//
	// Function: HandleGameMenuClick
	//
	// Handles the item click by loading the ruleset, if needed, then calling
	// the plugin click handler.
	//
	// Parameters:
	//     index - The index of the item being clicked.
	//     manager - The window manager so plugins can add panes.
	//     item - The menu item clicked.
	//
	virtual void HandleGameMenuClick(size_t index, wxAuiManager& manager,
		wxMenuItem* item)=0;

	//-----------------------------------------------------------------------//
	// Function: GetMainMenuItems
	//
	// Retrieves the map of main menu items.
	//
	// Returns:
	//     The map of main menu items.
	//
	virtual const MenuItemMap& GetMainMenuItems() const=0;

	//-----------------------------------------------------------------------//
	// Function: GetGameMenuItems
	//
	// Retrieves the map of game menu items.
	//
	// Paramters:
	//     items - The map of game menu items.
	//
	virtual void GetGameMenuItems(MenuItemMap& items) const=0;

	//-----------------------------------------------------------------------//
	// Function: RegisterMainMenuItem
	//
	// Registers a main menu item.  Plugins can provide menu hook via this 
	// interface.
	//
	// Parameters:
	//     plugin - The name of the plugin that handles the menu item.
	//     ruleset - The ruleset within the plugin.
	//     category - The main menu category.
	//     priority - The priority within that category.
	//     text - The text associated with the item.
	//     image - The image associated with the item.
	//     key - The key that identifies the menu item to the plugin.
	//
	virtual void RegisterMainMenuItem(const wxString& plugin, 
		const wxString& ruleset, const wxString& category, wxInt32 priority,
		const wxString& text, const HashString& image, const wxString& key)=0;

	//-----------------------------------------------------------------------//
	// Function: RegisterGameMenuItem
	//
	// Registers a main game item.  Plugins can provide menu hook via this 
	// interface.
	//
	// Parameters:
	//     plugin - The name of the plugin that handles the menu item.
	//     ruleset - The ruleset within the plugin.
	//     category - The game menu category.
	//     priority - The priority within that category.
	//     text - The text associated with the item.
	//     key - The key that identifies the menu item to the plugin.
	//     flags - Any specific menu flags for the item.
	//
	virtual void RegisterGameMenuItem(const wxString& plugin, 
		const wxString& ruleset, const wxString& category, wxInt32 priority,
		const wxString& text, const wxString& key,
		wxItemKind flags = wxITEM_NORMAL)=0;

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~IUIEngine
	// 
	// The IUIEngine destructor.  It is pure virtual because
	// this class must be derived from in order to be used.
	// 
	virtual ~IUIEngine()=0;
};

//---------------------------- PROTOTYPES       -----------------------------//
inline IUIEngine::~IUIEngine() {}
