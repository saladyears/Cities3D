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
#include "IStatEngine.h"
#include "Access.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
struct Stat;
typedef boost::shared_ptr<Stat> StatPtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: StatEngine
// 
// TODO: Fill this in.
//
// Derived From:
//     <IStatEngine>
//
// Project:
//     <RulesCore>
//
// Include:
//     StatEngine.h
//
class RULES_EXPORT_SPEC StatEngine 
	: public IStatEngine
	, public Access
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
    //-----------------------------------------------------------------------//
	// Group: Constructors
	//

    //-----------------------------------------------------------------------//
	// Constructor: StatEngine
	//
    // The StatEngine constructor.
    //
    StatEngine();

    //-----------------------------------------------------------------------//
	// Destructor: ~StatEngine
	//
    // The StatEngine destructor.
    //
    ~StatEngine();

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//
    
	virtual void ProcessRule(const HashString& rule, const DataObject& object);

    virtual void UnprocessRule(const HashString& rule);

	virtual void GetJson(json_spirit::wObject& obj, bool forceEnglish = false);

	virtual void save(wxDataOutputStream& stream) const;
	virtual void load(wxDataInputStream& stream);

	//-----------------------------------------------------------------------//
	// Group: Functions
	//
    
    //-----------------------------------------------------------------------//
	// Function: Stop
	//
	// Clears all existing stat groups and records.
	//
	void Stop();

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//	
	// Group: Functions
	//

	//-----------------------------------------------------------------------//	
	// Function: PushModifier
	//
	// Pushes any modifier for the current rule onto the modifier stack.
	//
	// Paramaters:
	//     rule - The rule to check.
	//
	void PushModifier(const HashString& rule);

	//-----------------------------------------------------------------------//	
	// Function: PopModifier
	//
	// Pops any modifier for the current rule off the modifier stack.
	//
	// Paramaters:
	//     rule - The rule to check.
	//
	void PopModifier(const HashString& rule);

	//-----------------------------------------------------------------------//	
	// Function: UpdateStats
	//
	// Updates the stat map with the given rule.
	//
	// Parameters:
	//     rule - The rule to update.
	//     object - The <DataObject> to use.
	//
	void UpdateStats(const HashString& rule, const DataObject& object);

	typedef std::map<wxInt32, HashString> ModifierDisplayMap;
	
	//-----------------------------------------------------------------------//	
	// Function: WriteDiceRollStat
	//
	// Writes a game stat to a json object.
	//
	// Parameters:
	//     name - The stat to write.
	//     obj - The JSON object.
	//     displayMap - The display map of modifiers.
	//     forceEnglish - Whether to use the English display string or not.
	//
	void WriteDiceRollStat(const HashString& stat, json_spirit::wObject& obj,
		const ModifierDisplayMap& displayMap, bool forceEnglish);

	//-----------------------------------------------------------------------//	
	// Function: WritePlayerStat
	//
	// Writes a player stat to a json object.
	//
	// Parameters:
	//     name - The stat to write.
	//     obj - The JSON object.
	//     displayMap - The display map of modifiers.
	//     forceEnglish - Whether to use the English display string or not.
	//
	void WritePlayerStat(const HashString& stat, json_spirit::wObject& obj,
		const ModifierDisplayMap& displayMap, bool forceEnglish);

    //-----------------------------------------------------------------------//	
	// Group: Game Event Functions
	//

    //-----------------------------------------------------------------------//	
	// Function: OnInitializeUI
	//
    // Loads stat defintions at the start of a game.
    //
    // Parameters:
    //     game - The current game.
    //
    void OnInitializeUI(const GamePtr& game);

	//-----------------------------------------------------------------------//
	// Section: Variables
	//

	typedef std::vector<StatPtr> StatArray;
    WX_DECLARE_HASH_STRING_HASH_MAP(StatArray, StatRuleMap);

    // The map of rules to stats that handle them.
    StatRuleMap mStatRuleMap;

    // The map of rules to stat modifiers.
    StatRuleMap mStatModifierMap;

	WX_DECLARE_HASH_STRING_HASH_MAP(StatPtr, StatNameMap);

	// The map of stat names to stats.
	StatNameMap mStatNameMap;

	typedef std::list<HashString> HashStringList;
    WX_DECLARE_HASH_STRING_HASH_MAP(HashStringList, ModifierMap);

    // The map of current modifiers.
    ModifierMap mModifierMap;

	typedef std::pair<HashString, ModifierDisplayMap> StatPair;
	typedef std::map<wxInt32, StatPair> StatDisplayMap;

	// The display map.
	StatDisplayMap mStatDisplayMap;

    WX_DECLARE_HASH_STRING_HASH_MAP(wxInt32, DiceRollStatModifierHash);
    WX_DECLARE_HASH_STRING_HASH_MAP(DiceRollStatModifierHash, DiceRollStatMap);

    typedef std::vector<wxInt32> PlayerStatArray;
    WX_DECLARE_HASH_STRING_HASH_MAP(PlayerStatArray, PlayerStatModifierHash);
    WX_DECLARE_HASH_STRING_HASH_MAP(PlayerStatModifierHash, PlayerStatMap);

    // The actual stats.
    DiceRollStatMap mDiceRollStatMap;
    PlayerStatMap mPlayerStatMap;

	wxInt32 mRuleDepth;
	bool mStatUpdate;
};

//---------------------------- PROTOTYPES       -----------------------------//

