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

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
struct BankData;
typedef boost::shared_ptr<BankData> BankDataPtr;

struct PlayerData;
typedef boost::shared_ptr<PlayerData> PlayerDataPtr;

struct BuildData;
typedef boost::shared_ptr<BuildData> BuildDataPtr;

struct Stat;
typedef boost::shared_ptr<Stat> StatPtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: IPluginEngine
// 
// The interface of the plugin engine.  The IPluginEngine is
// responsible for managing all of the plugin objects that contain
// <RuleSets>.  It has functions for loading
// specific <RuleSets> and for checking to see if certain
// <RuleSets> have already been loaded.  It also provides
// member functions to retrieve <BankData> and <PlayerData> out of
// the currently loaded <RuleSets>.
//
// When the IPluginEngine loads a <RuleSet>, it copies all of the
// component parts of the <RuleSet> into their respective destinations.  
// All <IRules> and <ILogic> go into the <IRuleEngine>.  All 
// <ImageSets> go into the <IImageEngine>.  And all <IDrawObjects> 
// go into the <IDrawObjectEngine>. The <IPluginEngine> is accessible 
// through the <GameEngine>.
//
// Project:
//     <RulesCore>
//
// Include:
//     IPluginEngine.h
//
class RULES_EXPORT_SPEC IPluginEngine
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: IsLoaded
	//
	// Determines if a plugin module is loaded.
	//
	// Parameters:
	//     module - The name of the plugin module to check.
	//
	// Returns:
	//     A bool stating if the module is loaded.  The value
	//     is true if it is, false if not.
	//
	virtual bool IsLoaded(const wxString &module)=0;

	//-----------------------------------------------------------------------//
	// Function: IsLoaded
	//
	// Determines if a <RuleSet> within a given module is loaded.
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
	virtual bool IsLoaded(const wxString &module, const wxString &ruleset)=0;

	//-----------------------------------------------------------------------//
	// Function: LoadRuleset
	//
	// Loads a <RuleSet> into the <GameEngine>.
	//
	// Parameters:
	//     module - The name of the module the <RuleSet> is in.
	//     ruleset - The <RuleSet> to load.
	//
	// Returns:
	//     A bool indicating the success of the load.  The value is true
	//     if the <RuleSet> was loaded, false if not.
	//
	virtual bool LoadRuleset(const wxString &module, 
		const wxString &ruleset)=0;

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
		const wxString &option)=0;

	//-----------------------------------------------------------------------//
	// Function: LoadBankData
	//
	// Retrieves <BankData> from all of the currently loaded <RuleSets>.
	//
	// Parameters:
	//     array - The outgoing array of <BankData> that will be filled
	//             by the function.
	//
	virtual void LoadBankData(std::vector<BankDataPtr> &array)=0;

	//-----------------------------------------------------------------------//
	// Function: LoadPlayerData
	//
	// Retreives <PlayerData> from all of the currently loaded <RuleSets>.
	//
	// Parameters:
	//     array - The outgoing array of <PlayerData> that will be filled
	//             by the function.
	//
	virtual void LoadPlayerData(std::vector<PlayerDataPtr> &array)=0;

	//-----------------------------------------------------------------------//
	// Function: LoadBuildData
	//
	// Retreives <BuildData> from all of the currently loaded <RuleSets>.
	//
	// Parameters:
	//     array - The outgoing array of <BuildData> that will be filled
	//             by the function.
	//
	virtual void LoadBuildData(std::vector<BuildDataPtr> &array)=0;

    //-----------------------------------------------------------------------//
	// Function: LoadStats
	//
	// Retreives <Stats> from all of the currently loaded <RuleSets>.
	//
	// Parameters:
	//     array - The outgoing array of <Stats> that will be filled
	//             by the function.
	//
	virtual void LoadStats(std::vector<StatPtr> &array)=0;

	typedef std::pair<wxString, wxString> StringPair;
	typedef std::vector<StringPair> StringPairArray;

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
	virtual void GetLoadedRuleSets(StringPairArray &array)=0;

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
	virtual const wxString& GetModuleName(const wxString& module)=0;

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
	virtual wxInt32 GetModulePriority(const wxString& module)=0;

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
		const wxString& ruleset)=0;

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~IPluginEngine
	// 
	// The IPluginEngine destructor.  It is pure virtual because
	// this class must be derived from in order to be used.
	// 
	virtual ~IPluginEngine()=0;
};

//---------------------------- PROTOTYPES       -----------------------------//
inline IPluginEngine::~IPluginEngine() {}

