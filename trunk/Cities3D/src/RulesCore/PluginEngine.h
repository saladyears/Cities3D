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
#include <wx/hashmap.h>
#include <map>
#include <list>

//---------------------------- USER INCLUDES    -----------------------------//
#include "RulesExport.h"
#include "IPluginEngine.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class RuleEngine;
class GameObjectEngine;
class DrawObjectEngine;
class UIEngine;
class OptionEngine;
class KeyMapEngine;

class wxDynamicLibrary;
typedef boost::shared_ptr<wxDynamicLibrary> DynamicLibraryPtr;

class RuleSet;
typedef boost::shared_ptr<RuleSet> RuleSetPtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: PluginEngine
// 
// The default implementation of the <IPluginEngine> interface.
// PluginEngine finds and keeps track of all plugin modules
// located in the /plugins subdirectory.  The functions required
// by the <IPluginEngine> interface are implemented using the
// various methods of storing the plugin module information.  See
// the function and variable documentation below.
//
// Derived From:
//     <IPluginEngine>
//
// Project:
//     <RulesCore>
//
// Include:
//     PluginEngine.h
//

class RULES_EXPORT_SPEC PluginEngine : public IPluginEngine
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: PluginEngine
	//
	// The PlugineEngine constructor.  Stores the given
	// references to other engines.
	//
	PluginEngine(RuleEngine &engine, GameObjectEngine &game, 
		DrawObjectEngine &draw, UIEngine& ui, OptionEngine& option,
		KeyMapEngine& keymap);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~PluginEngine
	//
	// The PluginEngine destructor.
	//
	~PluginEngine();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Start
	//
	// Starts the PluginEngine.  Calls <FindPlugins> to
	// locate available plugin modules.  Then loads the
	// Core <RuleSet> out of the <CoreRules> module.
	//
	void Start();

	//-----------------------------------------------------------------------//
	// Function: Stop
	//
	// Stops the PluginEngine.  Clears all <RuleSet>
	// paths.  Clears all loaded <RuleSet> names and
	// modules.
	//
	void Stop();

	//-----------------------------------------------------------------------//
	// Function: GetLoadedRuleSets
	//
	// Fills the given array with all loaded <RuleSets>.
	// The <RuleSets> are stored in the array in a std::pair
	// where the first element is the name of the module the
	// <RuleSet> is in, and the second element is the name of
	// the <Ruleset> itself.
	//
	// Parameters:
	//     array - The array to fill with loaded <RuleSet> information.
	//
	void GetLoadedRuleSets(StringPairArray &array);
	
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: IsLoaded
	//
	// Determines if a plugin module is loaded.  Checks
	// in the <RuleSet> PluginMap for a module with
	// the given name.  If an entry is found, the <RuleSet>
	// has been loaded.
	//
	// Parameters:
	//     module - The name of the plugin module to check.
	//
	// Returns:
	//     A bool stating if the module is loaded.  The value
	//     is true if it is, false if not.
	//
	virtual bool IsLoaded(const wxString &module);

	//-----------------------------------------------------------------------//
	// Function: IsLoaded
	//
	// Determines if a <RuleSet> within a given module is loaded.
	// First calls the module-only version of <IsLoaded> to see
	// if the module has been loaded.  If so, checks in the
	// std::list of loaded <RuleSets> for that module name to see if
	// the given <RuleSet> has been loaded.
	//
	// Parameters:
	//     module - The name of the module the <RuleSet> is in.
	//     ruleset - The name of the <RuleSet> to check.
	//
	// Returns:
	//     A bool stating if the <RuleSet> is loaded.  The value is true
	//     if both the module and <RuleSet> or loaded, and false if either
	//     the <RuleSet>, or the module is not loaded.
	//
	virtual bool IsLoaded(const wxString &module, const wxString &ruleset);

	//-----------------------------------------------------------------------//
	// Function: LoadRuleset
	//
	// Loads a <RuleSet> into the <GameEngine>.  Immediately checks
	// to see if the module is even available to load.  If yes, and the
	// module has not been loaded into memory, it is loaded via
	// wxWidget's wxDynamicLibrary class.  The loaded module is then
	// queried for the ProvideRuleSets function.  If the module fails to
	// load, or fails to provide the proper access function, LoadRuleset
	// fails and returns false.
	//
	// Once the ProvideRuleSets function has been located, a const reference
	// to a std::map of <RuleSets> is created from the return value of the 
	// ProvideRuleSets function.  The std::map is searched for the name of the 
	// <RuleSet> to load.  If the module does not provide the <RuleSet> (i.e., 
	// it is not in the std::map), the function fails and returns false.  If 
	// the module provides the given <RuleSet>, it is then loaded into the 
	// <GameEngine> by calling <LoadRulesetPrivate>.
	//
	// Parameters:
	//     module - The name of the module the <RuleSet> is in.
	//     ruleset - The <RuleSet> to load.
	//
	// Returns:
	//     A bool indicating the success of the load.  The value is true
	//     if the <RuleSet> was loaded, false if not.
	//
	virtual bool LoadRuleset(const wxString &module, const wxString &ruleset);

	//-----------------------------------------------------------------------//
	// Function: LoadOption
	//
	// Loads an option <RuleSet> so the <IOptionEngine> can access it.
	//
	// Parameters:
	//     module - The name of the module the option is in.
	//     option - The option to load.
	//
	// Returns:
	//     A bool indicating the success of the load.  The value is true
	//     if the <RuleSet> was loaded, false if not.
	//
	virtual bool LoadOption(const wxString &module, 
		const wxString &option);

	//-----------------------------------------------------------------------//
	// Function: LoadBankData
	//
	// Retrieves <BankData> from all of the currently loaded <RuleSets>.
	// Searches through all loaded <RuleSets>, checking to see if they
	// provide any <BankData>.  If <BankData> is provided by a <RuleSet>,
	// it is appended to the outgoing std::vector.
	//
	// Parameters:
	//     array - The outgoing array of <BankData> that will be filled
	//             by the function.
	//
	virtual void LoadBankData(std::vector<BankDataPtr> &array);

	//-----------------------------------------------------------------------//
	// Function: LoadPlayerData
	//
	// Retreives <PlayerData> from all of the currently loaded <RuleSets>.
	// Searches through all loaded <RuleSets>, checking to see if they
	// provide any <PlayerData>.  If <PlayerData> is provided by a <RuleSet>,
	// it is appended to the outgoing std::vector.
	//
	// Parameters:
	//     array - The outgoing array of <PlayerData> that will be filled
	//             by the function.
	//
	virtual void LoadPlayerData(std::vector<PlayerDataPtr> &array);
	
	//-----------------------------------------------------------------------//
	// Function: LoadBuildData
	//
	// Retreives <BuildData> from all of the currently loaded <RuleSets>.
	// Searches through all loaded <RuleSets>, checking to see if they
	// provide any <BuildData>.  If <BuildData> is provided by a <RuleSet>,
	// it is appended to the outgoing std::vector.
	//
	// Parameters:
	//     array - The outgoing array of <BuildData> that will be filled
	//             by the function.
	//
	virtual void LoadBuildData(std::vector<BuildDataPtr> &array);

    //-----------------------------------------------------------------------//
	// Function: LoadStats
	//
	// Retreives <Stats> from all of the currently loaded <RuleSets>.
	//
	// Parameters:
	//     array - The outgoing array of <Stats> that will be filled
	//             by the function.
	//
	virtual void LoadStats(std::vector<StatPtr> &array);

	//-----------------------------------------------------------------------//
	// Function: GetModuleName
	//
	// Returns the user-friendly name of the given module.
	//
	// Parameters:
	//     module - The module.
	//
	// Returns:
	//     The user-friendly name of the module.
	//
	virtual const wxString& GetModuleName(const wxString& module);

		//-----------------------------------------------------------------------//
	// Function: GetModulePriority
	//
	// Returns the display priority of the given module.
	//
	// Parameters:
	//     module - The module.
	//
	// Returns:
	//     The display priority of the module.
	//
	virtual wxInt32 GetModulePriority(const wxString& module);

	//-----------------------------------------------------------------------//
	// Function: GetRulesetName
	//
	// Returns the user-friendly name of the given ruleset.
	//
	// Parameters:
	//     module - The module.
	//     ruleset - The ruleset.
	//
	// Returns:
	//     The user-friendly name of the ruleset.
	//
	virtual const wxString& GetRulesetName(const wxString& module,
		const wxString& ruleset);

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//
	// The default PluginEngine constructor.  Declared private so
	// that the PluginEngine is always created with pointers to the
	// other game engines.
	//
	PluginEngine();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//
	
	//-----------------------------------------------------------------------//
	// Function: LoadRulesetPrivate
	//
	// Loads data from the given <RuleSet> into the <GameEngine>.
	// First appends the <RuleSet> name and module name to the 
	// std::vector of std::lists of open rulesets to record the
	// loading of the <RuleSet> and the order in which it happened.
	// Then locks the boost::weak_ptrs to the <RuleEngine>, and
	// <DrawObjectEngine> so that they can be used.
	// Copies all of the <IRules>, <ILogic>, mixin strings, and branch
	// priority <ILogic> from the <RuleSet> into the <RuleEngine>.  Copies
	// all of the <IGameObjects> from the <RuleSet> into the 
	// <GameObjectEngine>. Copies all of the <IDrawObjects> from the <RuleSet> 
	// into the <DrawObjectEngine>.  Copies all of the <ImageSets> from the 
	// <RuleSet> into the <ImageEngine>.
	//
	// Parameters:
	//     ruleset - The <RuleSet> being loaded.
	//     module - The name of the module the <RuleSet> is in.
	//     name - The name of the <RuleSet>.
	//     
	void LoadRulesetPrivate(const RuleSetPtr &ruleset, const wxString &module, 
		const wxString &name);

	//-----------------------------------------------------------------------//
	// Function: FindPlugins
	//
	// Finds plugin modules in the /plugins directory.  Examines each
	// file that ends in .dll on Win32 or .so on Linux, within the
	// plugins subdirectory.  Examines each dynamic library to see if
	// it provides a ProvideRuleSets function.  If a function
	// is provided, the module name and path to the module are added
	// as entries in the PluginMaps of <RuleSets>.
	//
	void FindPlugins();

	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnLoadRulesets
	//
	// Callback for when games are loaded from disk or network. Loads all of 
	// the <Rulesets> in the given array.
	//
	// Parameters:
	//     array - The container of module/<RuleSet> names to load for the 
	//             game being opened.
	//
	void OnLoadRulesets(const StringPairArray &array);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//
	
	typedef std::map<wxString, wxString> StringMap;

	//-----------------------------------------------------------------------//
	// Object: mRulesetPaths
	//
	// The std::map of the names of <RuleSet>-providing 
	// modules to the paths to those modules.
	//
	StringMap mRulesetPaths;

	//-----------------------------------------------------------------------//
	// Object: mOptionPaths
	//
	// The std::map of the names of <RuleSet>-providing 
	// modules to the paths to those modules.
	//
	StringMap mOptionPaths;

	//-----------------------------------------------------------------------//
	// Object: mModuleNames
	//
	// The map of user-friendly names of each module.
	//
	StringMap mModuleNames;

	//-----------------------------------------------------------------------//
	// Object: mRulesetNames
	//
	// The map of user-friendly names of each ruleset.
	//
	StringMap mRulesetNames;

	typedef std::map<wxString, wxInt32> PriorityMap;

	//-----------------------------------------------------------------------//
	// Object: mModulePriorities
	//
	// The map of display order priorities for each module.
	//
	PriorityMap mModulePriorities;

	//any plugins that are currently being used must be kept open, so they are 
	//stored in this list until they are not needed
	typedef std::map<wxString, DynamicLibraryPtr> PluginMap;

	//-----------------------------------------------------------------------//
	// Object: mRulePlugins
	//
	// The std::map of <RuleSet>-providing module names to the 
	// wxDynamicLibrary containing the loaded module.  A wxDynamicLibrary 
	// unloads its module from memory when it is destroyed, which means 
	// that the destruction of the std::map must happen after all previous
	// memory shared across address spaces has been cleaned up.
	//
	PluginMap mRulePlugins;

	//-----------------------------------------------------------------------//
	// Object: mOptionPlugins
	//
	// Same thing for options.
	//
	PluginMap mOptionPlugins;

	//the list of what module/rulesets we have open in the game
	typedef std::list<wxString> StringList;
	typedef std::pair<wxString, StringList> StringListPair;
	typedef std::vector<StringListPair> StringListPairArray;

	//-----------------------------------------------------------------------//
	// Object: mLoadedRulesets
	//
	// The std::vector of loaded <RuleSets>, grouped by
	// the module they were loaded from, and preserved in
	// load order.
	// 
	StringListPairArray mLoadedRulesets;

	//-----------------------------------------------------------------------//
	// Pointer: mRuleEngine
	//
	// The const reference to the <RuleEngine> that will receive
	// <IRule> and <ILogic> data from any <RuleSets> being loaded.
	//
	RuleEngine &mRuleEngine;

	//-----------------------------------------------------------------------//
	// Pointer: mGameObjectEngine
	//
	// The const reference to the <GameObjectEngine> that will receive
	// <IGameObject> data from any <RuleSets> being loaded.
	//
	GameObjectEngine &mGameObjectEngine;

	//-----------------------------------------------------------------------//
	// Pointer: mDrawObjectEngine
	//
	// The const reference to the <DrawObjectEngine> that will receive
	// <IDrawObject> data from any <RuleSets> being loaded.
	//
	DrawObjectEngine &mDrawObjectEngine;

	//-----------------------------------------------------------------------//
	// Pointer: mUIEngine
	//
	// The reference to the <UIEngine> that will receive <IUIPlugins> from any
	// <RuleSets> being loaded.
	//
	UIEngine& mUIEngine;

	//-----------------------------------------------------------------------//
	// Pointer: mOptionEngine
	//
	// The reference to the <OptionEngine> that will keep track of
	// options from loaded <RuleSets>.
	//
	OptionEngine& mOptionEngine;

	//-----------------------------------------------------------------------//
	// Pointer: mKeyMapEngine
	//
	// The reference to the <KeyMapEngine> that will keep track of keymaps
	// from loaded <RuleSets>.
	//
	KeyMapEngine& mKeyMapEngine;
};

//---------------------------- PROTOTYPES       -----------------------------//

