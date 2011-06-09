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
#include <set>

//---------------------------- USER INCLUDES    -----------------------------//
#include "IOptionEngine.h"
#include "IUIEngine.h"
#include "RuleSet.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: OptionEngine
// 
// The default implementation of the <IOptionEngine> interface.
//
// Derived From:
//     <IOptionEngine>
//
// Project:
//     <RulesCore>
//
// Include:
//     OptionEngine.h
//
class RULES_EXPORT_SPEC OptionEngine : public IOptionEngine
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructor
	//

	//-----------------------------------------------------------------------//
	// Constructor: OptionEngine
	//
	// The OptionEngine constructor.
	//
	OptionEngine();

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~OptionEngine
	//
	// The OptionEngine destructor.
	//
	~OptionEngine();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Stop
	//
	// Unloads all options.
	//
	void Stop();

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: GetOptions
	//
	// Returns all options available to the game.
	//
	// Returns:
	//     A map of all options available to the game.
	//
	virtual void GetOptions(RulesetPriorityMap& options) const;

	//-----------------------------------------------------------------------//
	// Function: GetSelectedOptions
	//
	// Returns all options selected for the current game.
	//
	// Returns:
	//     An array of all options selected for the current game.
	//
	virtual void GetSelectedOptions(StringPairArray& options) const;

	//-----------------------------------------------------------------------//
	// Function: SetSelectedOptions
	//
	// Sets all options selected for the current game.
	//
	// Parameters:
	//     options - The options selected for the current game.
	//
	virtual void SetSelectedOptions(const StringPairArray& options);

	//-----------------------------------------------------------------------//
	// Function: HasSelectedOptions
	//
	// Returns whether any options are currently selected.
	//
	// Returns a bool that is true if there are selected options and false if
	// not.
	//
	virtual bool HasSelectedOptions() const;

	//-----------------------------------------------------------------------//
	// Function: GetOptionStrings
	//
	// Returns the short strings describing each selected option.
	//
	// Parameters:
	//     pieces - [out] The array of strings describing each selected option.
	//
	virtual void GetOptionStrings(StringArray& pieces) const;

	//-----------------------------------------------------------------------//
	// Function: GetOptionUI
	//
	// Returns the <IOptionUI> for the given ruleset.
	//
	// Parameters:
	//     module - The module of the option.
	//     ruleset - The option ruleset.
	//
	// Returns:
	//     The ruleset's <IOptionUI>
	//
	virtual OptionUIPtr GetOptionUI(const wxString& module,
		const wxString& ruleset) const;

	//-----------------------------------------------------------------------//
	// Function: IsSelected
	//
	// Returns whether the given option is selected or not.
	//
	// Parameters:
	//     module - The module of the option.
	//     ruleset - The option ruleset.
	//
	// Returns:
	//     A bool that is true if the option is selected and false if not.
	//
	virtual bool IsSelected(const wxString& module, 
		const wxString& ruleset) const;

	//-----------------------------------------------------------------------//
	// Function: IsExcluded
	//
	// Returns whether the given option is excluded or not.
	//
	// Parameters:
	//     module - The module of the option.
	//     ruleset - The option ruleset.
	//
	// Returns:
	//     A bool that is true if the option is excluded and false if not.
	//
	virtual bool IsExcluded(const wxString& module, 
		const wxString& ruleset) const;

	//-----------------------------------------------------------------------//
	// Function: ClearOptions
	//
	// Clears all currently selected options.
	//
	virtual void ClearOptions();

	//-----------------------------------------------------------------------//
	// Function: SelectOption
	//
	// Selects an option for the game.
	//
	// Parameters:
	//     module - The module of the option.
	//     ruleset - The option ruleset.
	//
	virtual void SelectOption(const wxString& module, 
		const wxString& ruleset);

	//-----------------------------------------------------------------------//
	// Function: UnselectOption
	//
	// Unselects an option for the game.
	//
	// Parameters:
	//     module - The module of the option.
	//     ruleset - The option ruleset.
	//
	virtual void UnselectOption(const wxString& module, 
		const wxString& ruleset);

	//-----------------------------------------------------------------------//
	// Function: UpdateOption
	//
	// Updates the given option.
	//
	// Parameters:
	//     option - The module + option string.
	//     key - The value to update.
	//     value - The value to update it to.
	//
	virtual void UpdateOption(const wxString& option, const wxString& key,
		const wxString& value);

	//-----------------------------------------------------------------------//
	// Function: LoadOptions
	//
	// Loads option <RuleSets> at the start of the game.
	//
	virtual void LoadOptions() const;

	//-----------------------------------------------------------------------//
	// Function: SaveOptionsToFile
	//
	// Saves currently loaded options to an .xml file.
	//
	// Parameters:
	//     path - The path to the .xml file.
	//
	virtual void SaveOptionsToFile(const wxString& path) const;

	//-----------------------------------------------------------------------//
	// Function: LoadOptionsFromFile
	//
	// Loads options from an .xml file..
	//
	// Parameters:
	//     path - The path to the .xml file.
	//
	virtual void LoadOptionsFromFile(const wxString& path);

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	typedef std::list<StringPair> StringPairList;

	void ProcessOption(const wxString& module, const wxString& name,
		const OptionUIPtr& option);

	void LoadOption(const wxString& module, const wxString& name,
		const OptionUIPtr& option);

	void OnLoadOptions(const StringPairArray& options);

	void OnLoadMap(wxInt32);

	void BuildSelectedExclusions();
	void BuildLoadedRulesets();

	void EnforceInteractions();

	void LoadSilentOptions() const;

	//-----------------------------------------------------------------------//
	// Group: Variables
	//
	RulesetPriorityMap mOptionMap;
	RulesetPriorityMap mSilentOptionMap;

	typedef std::map<wxString, OptionUIPtr> LoadedOptionMap;
	LoadedOptionMap mLoadedOptionMap;

	typedef std::pair<wxString, wxString> StringPair;
	typedef std::vector<StringPair> StringPairArray;
	WX_DECLARE_STRING_HASH_MAP(StringPairArray, StringArrayHash);
	WX_DECLARE_STRING_HASH_MAP(StringArrayHash, InteractionHash);
	
	InteractionHash mExclusions;
	InteractionHash mDependencies;
	InteractionHash mSilentDependencies;

	InteractionHash mSilentOptionExclusions;
	InteractionHash mSilentOptionDependencies;

	typedef std::map<wxInt32, StringPairArray> StringPriorityMap;
	mutable StringPriorityMap mSelectedOptions;

	typedef std::set<wxString> StringSet;
	StringSet mSelectedExclusions;
	StringSet mLoadedRulesets;

	bool mSkipEnforcement;

	friend class PluginEngine;
	friend class UIEngine;
};

//---------------------------- PROTOTYPES       -----------------------------//

