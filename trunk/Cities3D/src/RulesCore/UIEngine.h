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
#include <set>
#include <list>

//---------------------------- USER INCLUDES    -----------------------------//
#include "RulesExport.h"
#include "IUIEngine.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class DataObject;
class OptionEngine;

class IUIPlugin;
typedef boost::shared_ptr<IUIPlugin> UIPluginPtr;

class RuleSet;
typedef boost::shared_ptr<RuleSet> RuleSetPtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: UIEngine
// 
// The default implementation of the <IUIEngine> interface.
//
// Derived From:
//     <IUIEngine>
//
// Project:
//     <RulesCore>
//
// Include:
//     UIEngine.h
//
class RULES_EXPORT_SPEC UIEngine : public IUIEngine
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: UIEngine
	//
	// The UIEngine constructor.
	//
	UIEngine();

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~UIEngine
	//
	// The UIEngine destructor.
	//
	~UIEngine();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Stop
	//
	// Unloads all <IUIPlugins>
	//
	void Stop();

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: SetMainFrame
	//
	// Sets the main application frame for use by all plugins.
	//
	// Parameters:
	//     window - The main frame.
	//
	virtual void SetMainFrame(wxWindow* window);

	//-----------------------------------------------------------------------//
	// Function: CreateUI
	//
	// Creates all UI for the game.
	//
	// Parameters:
	//     game - The current <Game>.
	//     manager - The AUI manager.
	//
	virtual void CreateUI(const GamePtr& game, wxAuiManager& manager);

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
		const;

	//-----------------------------------------------------------------------//
	// Function: RegisterUI
	//
	// Registers a UI with the engine.
	//
	// Parameters:
	//     ui - The UI.
	//
	virtual void RegisterUI(wxWindow* ui)
	{
		mUI.insert(ui);
	}

	//-----------------------------------------------------------------------//
	// Function: UnregisterUI
	//
	// Unregisters a UI with the engine.
	//
	// Parameters:
	//     ui - The UI.
	//
	virtual void UnregisterUI(wxWindow* ui)
	{
		UISet::iterator it = mUI.find(ui);
		if(mUI.end() != it)
		{
			mUI.erase(it);
		}
	}

	//-----------------------------------------------------------------------//
	// Function: HasRegisteredUI
	//
	// Returns whether there are any UI registered with the engine.
	//
	// Returns:
	//     A bool that is true if there are UI registered and false if not.
	//
	virtual bool HasRegisteredUI() const
	{
		return (false == mUI.empty());
	}

	//-----------------------------------------------------------------------//
	// Function: SetGLCanvas
	//
	// Sets the GL canvas so created UI may have access to GL-rendered bitmaps.
	//
	// Parameters:
	//     canvas - The GL canvas.
	//
	virtual void SetGLCanvas(wxSettlersGLCanvas* window);

	//-----------------------------------------------------------------------//
	// Function: CanQuit
	//
	// Tests to see if the user is allowed to quit the game.
	//
	// Parameters:
	//     window - The window sending the quit message.
	//
	virtual bool CanQuit(wxWindow* window);

	//-----------------------------------------------------------------------//
	// Function: HandleMainMenuClick
	//
	// Handles the item click by loading the ruleset, if needed, then calling
	// the plugin click handler.
	//
	// Parameters:
	//     index - The index of the item being clicked.
	//
	virtual void HandleMainMenuClick(size_t index);

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
		wxMenuItem* item);

	//-----------------------------------------------------------------------//
	// Function: GetMainMenuItems
	//
	// Retrieves the map of main menu items.
	//
	// Returns:
	//     The map of main menu items.
	//
	virtual const MenuItemMap& GetMainMenuItems() const
	{
		return mMainMenuItemMap;
	}

	//-----------------------------------------------------------------------//
	// Function: GetGameMenuItems
	//
	// Retrieves the map of game menu items.
	//
	// Paramters:
	//     items - The map of game menu items.
	//
	virtual void GetGameMenuItems(MenuItemMap& items) const;

	//-----------------------------------------------------------------------//
	// Function: RegisterMainMenuItem
	//
	// Registers a main menu item.  Plugins can provide main menu hooks via
	// this interface.
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
		const wxString& text, const HashString& image, const wxString& key);

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
		wxItemKind flags = wxITEM_NORMAL);

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: AddPlugin
	//
	// Adds a plugin to the list.
	//
	// Parameters:
	//     plugin - The plugin.
	//     module - The module it came from.
	//     ruleset - The ruleset it came from.
	//
	void AddPlugin(const UIPluginPtr& plugin, const wxString& module, 
		const wxString& ruleset);

	//-----------------------------------------------------------------------//
	// Function: ProcessRuleSet
	//
	// Builds a list of all low-level exclusions that the <RuleSets> have.
	//
	// Parameters:
	//     module - The module.
	//     name - The name of the ruleset.
	//     ruleset - The ruleset itself.
	//
	void ProcessRuleSet(const wxString& module, const wxString& name, 
		const RuleSetPtr& ruleset);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Int: mMainMenuIndex
	//
	// The current main menu index.
	//
	size_t mMainMenuIndex;

	//-----------------------------------------------------------------------//
	// Int: mGameMenuIndex
	//
	// The current game menu index.
	//
	size_t mGameMenuIndex;

	//-----------------------------------------------------------------------//
	// Int: mQuitHandlerPriority
	//
	// The highest quit handler priority.
	//
	wxInt32 mQuitHandlerPriority;

	//-----------------------------------------------------------------------//
	// Pointer: mMainFrame
	//
	// The main application frame.
	//
	wxWindow* mMainFrame;

	typedef std::map<wxString, UIPluginPtr> UIPluginMap;

	//-----------------------------------------------------------------------//
	// Object: mUIPluginMap
	//
	// The map of <IUIPlugins> loaded.
	//
	UIPluginMap mUIPluginMap;

	typedef boost::function1<bool, wxWindow*> QuitHandler;
	
	//-----------------------------------------------------------------------//
	// Object: mQuitHandler
	//
	// The quit handler with the highest priority provided by <RuleSets>.
	//
	QuitHandler mQuitHandler;

	//-----------------------------------------------------------------------//
	// Object: mMainMenuItemMap
	//
	// The map of main menu items provided by plugins.
	//
	MenuItemMap mMainMenuItemMap;

	//-----------------------------------------------------------------------//
	// Object: mGameMenuItemMap
	//
	// The map of game menu items provided by plugins.
	//
	MenuItemMap mGameMenuItemMap;

	//-----------------------------------------------------------------------//
	// Object: mMainMenuIndexMap
	//
	// The cache of main menu items to wxWidgets control IDs.
	//
	typedef std::map<size_t, MenuItem> MenuIndexMap;
	MenuIndexMap mMainMenuIndexMap;

	//-----------------------------------------------------------------------//
	// Object: mGameMenuIndexMap
	//
	// The cache of game menu items to wxWidgets control IDs.
	//
	MenuIndexMap mGameMenuIndexMap;

	typedef std::pair<wxString, wxString> StringPair;
	typedef std::vector<StringPair> StringPairArray;
	WX_DECLARE_STRING_HASH_MAP(StringPairArray, StringArrayHash);
	WX_DECLARE_STRING_HASH_MAP(StringArrayHash, InteractionHash);

	//-----------------------------------------------------------------------//
	// Object: mExclusions
	//
	// The hash of low-level <RuleSet> exclusions.
	//
	InteractionHash mExclusions;

	typedef std::set<wxWindow*> UISet;
	UISet mUI;

	friend class PluginEngine;
};

//---------------------------- PROTOTYPES       -----------------------------//

