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
#include <map>

//---------------------------- USER INCLUDES    -----------------------------//
#include "RulesExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class RuleSet;
typedef boost::shared_ptr<RuleSet> RuleSetPtr;

class IOptionUI;
typedef boost::shared_ptr<IOptionUI> OptionUIPtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: IOptionEngine
// 
// The interface of the game option engine.  The IOptionEngine is responsible
// for managing which options are usable by the players for the current game.
//
// Project:
//     <RulesCore>
//
// Include:
//     IOptionEngine.h
//
class RULES_EXPORT_SPEC IOptionEngine
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	typedef std::map<wxInt32, wxString> OptionPriorityMap;
	typedef std::map<wxString, OptionPriorityMap> RulesetOptionMap;
	typedef std::map<wxInt32, RulesetOptionMap> RulesetPriorityMap;

	typedef std::pair<wxString, wxString> StringPair;
	typedef std::vector<StringPair> StringPairArray;

	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: GetOptions
	//
	// Returns all options available to the game.
	//
	// Parameters:
	//     options - [out] A map of all options available to the game.
	//
	virtual void GetOptions(RulesetPriorityMap& options) const=0;

	//-----------------------------------------------------------------------//
	// Function: GetSelectedOptions
	//
	// Returns all options selected for the current game.
	//
	// Parameters:
	//     options - [out] The options selected for the current game.
	//
	virtual void GetSelectedOptions(StringPairArray& options) const=0;

	//-----------------------------------------------------------------------//
	// Function: SetSelectedOptions
	//
	// Sets all options selected for the current game.
	//
	// Parameters:
	//     options - The options selected for the current game.
	//
	virtual void SetSelectedOptions(const StringPairArray& options)=0;

	//-----------------------------------------------------------------------//
	// Function: HasSelectedOptions
	//
	// Returns whether any options are currently selected.
	//
	// Returns a bool that is true if there are selected options and false if
	// not.
	//
	virtual bool HasSelectedOptions() const=0;

	//-----------------------------------------------------------------------//
	// Function: GetOptionStrings
	//
	// Returns the short strings describing each selected option.
	//
	// Parameters:
	//     pieces - [out] The array of strings describing each selected option.
	//
	virtual void GetOptionStrings(StringArray& pieces) const=0;

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
		const wxString& ruleset) const=0;

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
		const wxString& ruleset) const=0;

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
		const wxString& ruleset) const=0;

	//-----------------------------------------------------------------------//
	// Function: ClearOptions
	//
	// Clears all currently selected options.
	//
	virtual void ClearOptions()=0;

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
		const wxString& ruleset)=0;

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
		const wxString& ruleset)=0;

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
		const wxString& value)=0;

	//-----------------------------------------------------------------------//
	// Function: LoadOptions
	//
	// Loads option <RuleSets> at the start of the game.
	//
	virtual void LoadOptions() const=0;

	//-----------------------------------------------------------------------//
	// Function: SaveOptionsToFile
	//
	// Saves currently loaded options to an .xml file.
	//
	// Parameters:
	//     path - The path to the .xml file.
	//
	virtual void SaveOptionsToFile(const wxString& path) const=0;

	//-----------------------------------------------------------------------//
	// Function: LoadOptionsFromFile
	//
	// Loads options from an .xml file..
	//
	// Parameters:
	//     path - The path to the .xml file.
	//
	virtual void LoadOptionsFromFile(const wxString& path)=0;

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~IOptionEngine
	// 
	// The IOptionEngine destructor.  It is pure virtual because
	// this class must be derived from in order to be used.
	// 
	virtual ~IOptionEngine()=0;
};

//---------------------------- PROTOTYPES       -----------------------------//
inline IOptionEngine::~IOptionEngine() {}
