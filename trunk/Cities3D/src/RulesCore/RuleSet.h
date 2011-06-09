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
#include <map>
#include <wx/hashmap.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "RulesExport.h"
#include "IRuleEngine.h"
#include "IKeyMapEngine.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class IGameObject;
typedef boost::shared_ptr<IGameObject> GameObjectPtr;

class IDrawObject;
typedef boost::shared_ptr<IDrawObject> DrawObjectPtr;

class ImageSet;
typedef boost::shared_ptr<ImageSet> ImageSetPtr;

struct BankData;
typedef boost::shared_ptr<BankData> BankDataPtr;

struct PlayerData;
typedef boost::shared_ptr<PlayerData> PlayerDataPtr;

struct BuildData;
typedef boost::shared_ptr<BuildData> BuildDataPtr;

class IUIPlugin;
typedef boost::shared_ptr<IUIPlugin> UIPluginPtr;

class IOptionUI;
typedef boost::shared_ptr<IOptionUI> OptionUIPtr;

struct Stat;
typedef boost::shared_ptr<Stat> StatPtr;

typedef std::pair<wxString, wxString> StringPair;
typedef std::vector<StringPair> StringPairArray;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleSet
// 
// A repository of all things rules-related.  A RuleSet is an aggregate
// container that can hold the many types of objects needed to run a certain
// set of rules in the game.  A RuleSet can range in size from a single <IRule>
// for a very small variant, to dozens of <IRules> and <ILogic>, custom
// <ImageSets>, special <BankData>, <PlayerData>, or <BuildData> for the 
// <BankUI>, <PlayerUI> and <BuildUI>, <IDrawObjects>, <IGameObjects>, and even 
// an entire system of <IRule> branching logic for core game rules or an 
// extensive variant, or anything in between.  Every RuleSet must have a unique 
// name (within its plugin object).
//
// The plugin rule objects (.dlls on Win32, .so files on Linux), provide the
// RuleSets that the core application has access to.  A plugin object has its
// own <RuleSetDatabase> that stores all of the RuleSets until they are needed
// (if ever) by the core application.  The <PluginEngine> queries all plugins
// that it can find and keeps record of them.  Certain <Maps> require certain
// RuleSets in order to be played.  The core application is responsible for
// ensuring that all RuleSets that a given <Map> needs are loaded before the
// game begins or continues from a saved spot.  See <PluginEngine>, 
// <RuleEngine>, and <RuleSetDatabase> for more details on how RuleSets are 
// used throughout the game.
//
// Project:
//     <RulesCore>
//
// Include:
//     RuleSet.h
//
class RULES_EXPORT_SPEC RuleSet
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: RuleSet
	//
	// The RuleSet constructor.  Sets the name of the RuleSet.
	//
	// Parameters:
	//     name - The name of the RuleSet.  Must be unique within
	//            the plugin object in which it resides.
	//
	RuleSet(const wxString &name);

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	WX_DECLARE_HASH_STRING_HASH_MAP(GameObjectPtr, GameObjectHash);
	typedef std::set<DrawObjectPtr> DrawObjectSet;
	typedef std::list<ImageSetPtr> ImageSetList;
	typedef std::list<BankDataPtr> BankDataList;
	typedef std::list<PlayerDataPtr> PlayerDataList;
	typedef std::list<BuildDataPtr> BuildDataList;
	typedef std::list<StatPtr> StatList;
	typedef std::pair<wxInt32, LogicPtr> PriorityLogicPair;
	typedef std::list<PriorityLogicPair> PriorityLogicList;
	WX_DECLARE_HASH_STRING_HASH_MAP(PriorityLogicList, PriorityLogicHash);

	//-----------------------------------------------------------------------//
	// Function: name
	//
	// Returns the user-friendly name of the RuleSet.
	//
	// Returns:
	//     The user-friendly name of the RuleSet.
	//
	const wxString& name() const
	{
		return mTranslatedName;
	}

	//-----------------------------------------------------------------------//
	// Function: rules_begin
	//
	// Provides an iterator to the start of the <IRule> hash.
	//
	// Returns:
	//     A const_iterator to the start of <IRule> hash.
	//
	IRuleEngine::RuleHash::const_iterator rules_begin() const 
	{
		return mRuleHash.begin();
	}

	//-----------------------------------------------------------------------//
	// Function: rules_end
	//
	// Provides an iterator to the end of the <IRule> hash.
	//
	// Returns:
	//     A const_iterator to the end of <IRule> hash.
	//
	IRuleEngine::RuleHash::const_iterator rules_end() const 
	{
		return mRuleHash.end();
	}

	//-----------------------------------------------------------------------//
	// Function: logic_begin
	//
	// Provides an iterator to the start of the <ILogic> hash.
	//
	// Returns:
	//     A const_iterator to the start of <ILogic> hash.
	//
	IRuleEngine::LogicHash::const_iterator logic_begin() const 
	{
		return mLogicHash.begin();
	}

	//-----------------------------------------------------------------------//
	// Function: logic_end
	//
	// Provides an iterator to the end of the <ILogic> hash.
	//
	// Returns:
	//     A const_iterator to the end of <ILogic> hash.
	//
	IRuleEngine::LogicHash::const_iterator logic_end() const 
	{
		return mLogicHash.end();
	}

	//-----------------------------------------------------------------------//
	// Function: mixins_begin
	//
	// Provides an iterator to the start of the mixin hash.
	//
	// Returns:
	//     A const_iterator to the start of mixin hash.
	//
	IRuleEngine::MixinHash::const_iterator mixins_begin() const 
	{
		return mMixinHash.begin();
	}

	//-----------------------------------------------------------------------//
	// Function: mixins_end
	//
	// Provides an iterator to the end of the mixin hash.
	//
	// Returns:
	//     A const_iterator to the end of mixin hash.
	//
	IRuleEngine::MixinHash::const_iterator mixins_end() const 
	{
		return mMixinHash.end();
	}

	//-----------------------------------------------------------------------//
	// Function: branch_begin
	//
	// Provides an iterator to the start of the PriorityLogicHash hash.
	//
	// Returns:
	//     A const_iterator to the start of prioritized rule branching <ILogic>
	//     hash.
	//
	PriorityLogicHash::const_iterator branch_begin() const 
	{
		return mBranchHash.begin();
	}

	//-----------------------------------------------------------------------//
	// Function: branch_end
	//
	// Provides an iterator to the end of the PriorityLogicHash hash.
	//
	// Returns:
	//     A const_iterator to the end of prioritized rule branching <ILogic> 
	//     hash.
	//
	PriorityLogicHash::const_iterator branch_end() const 
	{
		return mBranchHash.end();
	}

	//-----------------------------------------------------------------------//
	// Function: gameobjects_begin
	//
	// Provides an iterator to the start of the <IGameObject> hash.
	//
	// Returns:
	//     A const_iterator to the start of the hash map of <IGameObjects> in 
	//     this RuleSet.
	//
	GameObjectHash::const_iterator gameobjects_begin() const 
	{
		return mGameObjectHash.begin();
	}

	//-----------------------------------------------------------------------//
	// Function: gameobjects_end
	//
	// Provides an iterator to the end of the <IGameObject> set.
	//
	// Returns:
	//     A const_iterator to the end of the hash map of <IGameObject> in this
	//     RuleSet.
	//
	GameObjectHash::const_iterator gameobjects_end() const 
	{
		return mGameObjectHash.end();
	}

	//-----------------------------------------------------------------------//
	// Function: drawobjects_begin
	//
	// Provides an iterator to the start of the <IDrawObject> set.
	//
	// Returns:
	//     A const_iterator to the start of the std::set of <IDrawObjects> in 
	//     this RuleSet.
	//
	DrawObjectSet::const_iterator drawobjects_begin() const 
	{
		return mDrawObjectSet.begin();
	}

	//-----------------------------------------------------------------------//
	// Function: drawobjects_end
	//
	// Provides an iterator to the end of the <IDrawObject> set.
	//
	// Returns:
	//     A const_iterator to the end of the std::set of <IDrawObjects> in 
	//     this RuleSet.
	//
	DrawObjectSet::const_iterator drawobjects_end() const 
	{
		return mDrawObjectSet.end();
	}

	//-----------------------------------------------------------------------//
	// Function: imagesets_begin
	//
	// Provides an iterator to the start of the <ImageSet> list.
	//
	// Returns:
	//     A const_iterator to the start of the std::list of <ImageSets> in 
	//     this RuleSet.
	//
	ImageSetList::const_iterator imagesets_begin() const 
	{
		return mImageSetList.begin();
	}

	//-----------------------------------------------------------------------//
	// Function: imagesets_end
	//
	// Provides an iterator to the end of the <ImageSet> list.
	//
	// Returns:
	//     A const_iterator to the end of the std::list of <ImageSets> in this
	//     RuleSet.
	//
	ImageSetList::const_iterator imagesets_end() const 
	{
		return mImageSetList.end();
	}

	//-----------------------------------------------------------------------//
	// Function: bankdata_begin
	//
	// Provides an iterator to the start of the <BankData> list.
	//
	// Returns:
	//     A const_iterator to the start of the std::list of <BankData> in this
	//     RuleSet.
	//
	BankDataList::const_iterator bankdata_begin() const 
	{
		return mBankDataList.begin();
	}

	//-----------------------------------------------------------------------//
	// Function: bankdata_end
	//
	// Provides an iterator to the end of the <BankData> list.
	//
	// Returns:
	//     A const_iterator to the end of the std::list of <BankData> in this 
	//     RuleSet.
	//
	BankDataList::const_iterator bankdata_end() const 
	{
		return mBankDataList.end();
	}

	//-----------------------------------------------------------------------//
	// Function: playerdata_begin
	//
	// Provides an iterator to the start of the <PlayerData> list.
	//
	// Returns:
	//     A const_iterator to the start of the std::list of <PlayerData> in 
	//     this RuleSet.
	//
	PlayerDataList::const_iterator playerdata_begin() const 
	{
		return mPlayerDataList.begin();
	}

	//-----------------------------------------------------------------------//
	// Function: playerdata_end
	//
	// Provides an iterator to the end of the <PlayerData> list.
	//
	// Returns:
	//     A const_iterator to the end of the std::list of <PlayerData> in this
	//     RuleSet.
	//
	PlayerDataList::const_iterator playerdata_end() const 
	{
		return mPlayerDataList.end();
	}

	//-----------------------------------------------------------------------//
	// Function: builddata_begin
	//
	// Provides an iterator to the start of the <BuildData> list.
	//
	// Returns:
	//     A const_iterator to the start of the std::list of <BuildData> in 
	//     this RuleSet.
	//
	BuildDataList::const_iterator builddata_begin() const 
	{
		return mBuildDataList.begin();
	}

	//-----------------------------------------------------------------------//
	// Function: builddata_end
	//
	// Provides an iterator to the end of the <BuildData> list.
	//
	// Returns:
	//     A const_iterator to the end of the std::list of <BuildData> in this
	//     RuleSet.
	//
	BuildDataList::const_iterator builddata_end() const 
	{
		return mBuildDataList.end();
	}

	//-----------------------------------------------------------------------//
	// Function: ui_plugin
	//
	// Returns the <IUIPlugin> for the RuleSet, if any.
	//
	// Returns:
	//     The <IUIPlugin> for the RuleSet.  May be NULL.
	//
	const UIPluginPtr& ui_plugin() const
	{
		return mUIPlugin;
	}

	//-----------------------------------------------------------------------//
	// Function: option_ui
	//
	// Returns the <OptionUIPtr> for the RuleSet, if any.
	//
	// Returns:
	//     The <OptionUIPtr> for the RuleSet.  May be NULL.
	//
	const OptionUIPtr& option_ui() const
	{
		return mOptionUI;
	}

	//-----------------------------------------------------------------------//
	// Function: exclusions
	//
	// Returns the exclusions of the RuleSet, if any.
	//
	// Returns:
	//     The exclusions, if any.
	//
	const StringPairArray& exclusions() const
	{
		return mExclusions;
	}

	//-----------------------------------------------------------------------//
	// Function: keymaps
	//
	// Returns the keymaps of the RuleSet, if any.
	//
	// Returns:
	//     The exclusions, if any.
	//
	const IKeyMapEngine::KeyMapArray& keymaps() const
	{
		return mKeyMaps;
	}

	//-----------------------------------------------------------------------//
	// Function: stats
	//
	// Returns the stats of the RuleSet, if any.
	//
	// Returns:
	//     The stats, if any.
	//
	const StatList& stats() const
	{
		return mStatList;
	}

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Copy Constructor
	//

	//-----------------------------------------------------------------------//
	// Constructor: RuleSet
	//
	// The RuleSet copy constructor.  Private and unimplemented because
	// a RuleSet cannot be copied.
	//
	RuleSet(const RuleSet &ruleset);

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: RegisterName
	//
	// Sets the translated name of the ruleset.
	//
	// Parameters:
	//     name - The translated name of the ruleset.
	//
	void RegisterName(const wxString& name)
	{
		mTranslatedName = name;
	}

	//-----------------------------------------------------------------------//
	// Function: RegisterRule
	//
	// Adds an <IRule> to the rule hash.  The given <IRule> is
	// stored in the hash with the given string name as its key.
	//
	// Parameters:
	//     rule - The <IRule> to add to the hash.
	//     name - The name of the <IRule>.  Used as the key for the
	//            <IRule> in the hash.
	//
	void RegisterRule(const RulePtr &rule, const HashString &name);

	//-----------------------------------------------------------------------//
	// Function: RegisterLogic
	//
	// Adds a piece of <ILogic> to the logic hash.  The given <ILogic> is
	// stored in the hash with the given string name as its key.
	//
	// Parameters:
	//     rule - The <ILogic> to add to the hash.
	//     name - The name of the <ILogic>.  Used as the key for the
	//            <ILogic> in the hash.
	//
	void RegisterLogic(const LogicPtr &logic, const HashString &name);

	//-----------------------------------------------------------------------//
	// Function: RegisterMixin
	//
	// Adds a mixin to the mixin hash.  The mixin name is inserted at
	// the given key into the hash map of mixins for the given rule name.
	//
	// Parameters:
	//     rule - The name of the <IRule> or <ILogic> the mixin name applies 
	//            to.
	//     key - The hash key at which to store the mixin rule name.
	//     mixin - The name of the mixin <IRule> or <ILogic> being applied.
	//
	void RegisterMixin(const HashString &rule, const HashString &key, 
		const HashString &mixin);

	//-----------------------------------------------------------------------//
	// Function: RegisterBranch
	//
	// Adds a piece of branch logic to the rule chain.  Branch
	// logic is used to determine what <IRule> will follow after
	// another <IRule>, when that kind of chaining is required.  (For
	// instance, Rule0 at the start of the game might usually go into
	// placing settlements, but for A New World maps, it should go into
	// placing ports.)  Each piece of branch <ILogic> can have an associated
	// priority number, and higher priority branch <ILogic> has precedence
	// over lower numbered priority <ILogic>.  All pieces of branch <ILogic>
	// should return a HashString of the next name of the <IRule> to execute,
	// or an empty string if it cannot decide with the given data.
	//
	// Parameters:
	//     rule - The rule to branch after.
	//     priority - The priority of this piece of branch <ILogic>.  Higher
	//                priority numbers have precedence.
	//     logic - The branch <ILogic>.
	//
	void RegisterBranch(const HashString &rule, const wxInt32 priority, 
		const LogicPtr &logic);

	//-----------------------------------------------------------------------//
	// Function: RegisterGameObject
	//
	// Adds an <IGameObject> to the RuleSet.  The <IGameObjects>
	// are stored in a hash so that when loading a saved game
	// from the disk, all <IGameObjects> can be instantiated
	// correctly.
	//
	// Parameters:
	//     name - The name of the <IGameObject>.
	//     object - The <IGameObject> to add.
	//
	void RegisterGameObject(const HashString &name, const GameObjectPtr &object);

	//-----------------------------------------------------------------------//
	// Function: RegisterDrawObject
	//
	// Adds an <IDrawObject> to the RuleSet.  Any <IGameObject> that needs
	// to be visually represented in the scene graph must have an <IDrawObject>
	// that knows how to draw it.
	//
	// Parameters:
	//     object - The <IDrawObject> to add.
	//
	void RegisterDrawObject(const DrawObjectPtr &object);

	//-----------------------------------------------------------------------//
	// Function: RegisterImageSet
	//
	// Adds an <ImageSet> to the RuleSet.  Some RuleSets might have their own
	// bitmap data that they want to use as OpenGL textures or UI bitmaps.
	// By providing an <ImageSet>, the RuleSet allows any other <IDrawObjects>,
	// <BankData>, or <PlayerData> that need the images access through the
	// <ImageEngine>.
	//
	// Parameters:
	//     imageset - The <ImageSet> to add.
	//
	void RegisterImageSet(const ImageSetPtr &imageset);

	//-----------------------------------------------------------------------//
	// Function: RegisterBankData
	//
	// Adds a piece of <BankData> to the RuleSet.  See <BankData> for more
	// details.
	//
	// Parameters:
	//     bankdata - The <BankData> to add.
	//
	void RegisterBankData(const BankDataPtr &bankdata);

	//-----------------------------------------------------------------------//
	// Function: RegisterPlayerData
	//
	// Adds a piece of <PlayerData> to the RuleSet.  See <PlayerData> for more
	// details.
	//
	// Parameters:
	//     playerdata - The <PlayerData> to add.
	//
	void RegisterPlayerData(const PlayerDataPtr &playerdata);

	//-----------------------------------------------------------------------//
	// Function: RegisterBuildData
	//
	// Adds a piece of <BuildData> to the RuleSet.  See <BuildData> for more
	// details.
	//
	// Parameters:
	//     builddata - The <BuildData> to add.
	//
	void RegisterBuildData(const BuildDataPtr &builddata);

	//-----------------------------------------------------------------------//
	// Function: SetUIPlugin
	//
	// Sets the <IUIPlugin> object for this RuleSet.
	//
	// Parameters:
	//     plugin - The <IUIPlugin> to set.
	//
	void SetUIPlugin(const UIPluginPtr& plugin);

	//-----------------------------------------------------------------------//
	// Function: SetOptionUI
	//
	// Sets the <OptionUIPtr> object for this RuleSet.
	//
	// Parameters:
	//     plugin - The <OptionUIPtr> to set.
	//
	void SetOptionUI(const OptionUIPtr& option);

	//-----------------------------------------------------------------------//
	// Function: RegisterExclusions
	//
	// Sets ruleset exclusions.
	//
	// Parameters:
	//     exclusions - The exclusions.
	//
	void RegisterExclusions(const StringPairArray& exclusions)
	{
		mExclusions = exclusions;
	}

	//-----------------------------------------------------------------------//
	// Function: RegisterKeyMap
	//
	// Registers a <IKeyMapEngine::KeyMap>.
	//
	// Parameters:
	//     entry - The key map.
	//
	void RegisterKeyMap(const IKeyMapEngine::KeyMap& entry)
	{
		mKeyMaps.push_back(entry);
	}

	//-----------------------------------------------------------------------//
	// Function: RegisterStat
	//
	// Registers a <Stat>.
	//
	// Parameters:
	//     stat - The stat.
	//
	void RegisterStat(const StatPtr& stat)
	{
		mStatList.push_back(stat);
	}

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Object: mName
	//
	// The name of this RuleSet.  Must be unique within its
	// plugin object.
	//
	wxString mName;

	//-----------------------------------------------------------------------//
	// Object: mTranslatedName
	//
	// The user-friendly display name of this ruleset.
	//
	wxString mTranslatedName;

	//-----------------------------------------------------------------------//
	// Hash: mRuleHash
	//
	// The string-based hash of <IRules>.
	//
	IRuleEngine::RuleHash mRuleHash;

	//-----------------------------------------------------------------------//
	// Hash: mLogicHash
	//
	// The string-based hash of <ILogic>.
	//
	IRuleEngine::LogicHash mLogicHash;

	//-----------------------------------------------------------------------//
	// Hash: mMixinHash
	//
	// The string-based hash of mixin strings.
	//
	IRuleEngine::MixinHash mMixinHash;

	//-----------------------------------------------------------------------//
	// Hash: mBranchHash
	//
	// The hash of rule branching logic.  See <RegisterBranch>
	// above for more details.
	//
	PriorityLogicHash mBranchHash;

	//-----------------------------------------------------------------------//
	// Object: mGameObjectHash
	//
	// The hash of <IGameObjects> the RuleSet provides.
	//
	GameObjectHash mGameObjectHash;

	//-----------------------------------------------------------------------//
	// Object: mDrawObjectSet
	//
	// The std::set of <IDrawObjects> the RuleSet provides.
	//
	DrawObjectSet mDrawObjectSet;

	//-----------------------------------------------------------------------//
	// Object: mImageSetList
	//
	// The std::list of <ImageSets> the RuleSet provides.
	//
	ImageSetList mImageSetList;

	//-----------------------------------------------------------------------//
	// Object: mBankDataList
	//
	// The std::list of <BankData> in the RuleSet.
	//
	BankDataList mBankDataList;

	//-----------------------------------------------------------------------//
	// Object: mPlayerDataList
	//
	// The std::list of <PlayerData> in the RuleSet.
	//
	PlayerDataList mPlayerDataList;

	//-----------------------------------------------------------------------//
	// Object: mBuildDataList
	//
	// The std::list of <BuildData> in the RuleSet.
	//
	BuildDataList mBuildDataList;

	//-----------------------------------------------------------------------//
	// Object: mUIPlugin
	//
	// The <IUIPlugin> for this RuleSet.
	//
	UIPluginPtr mUIPlugin;
	
	//-----------------------------------------------------------------------//
	// Object: mOptionUI
	//
	// The <OptionUIPtr> for this RuleSet.
	//
	OptionUIPtr mOptionUI;

	//-----------------------------------------------------------------------//
	// Object: mExclusions
	//
	// The list of other rulesets this ruleset is not allowed to be played 
	// with.
	//
	StringPairArray mExclusions;

	//-----------------------------------------------------------------------//
	// Object: mKeyMaps
	//
	// The list of <IKeyMapEngine::KeyMap> entries for this RuleSet.
	//
	IKeyMapEngine::KeyMapArray mKeyMaps;

	//-----------------------------------------------------------------------//
	// Object: mStatList
	//
	// The list of <Stats> for this RuleSet.
	//
	StatList mStatList;

	//so that only accessors are public
	friend class RuleSetDatabase;
	friend class OptionDatabase;
};

typedef boost::shared_ptr<RuleSet> RuleSetPtr;
typedef std::map<wxString, RuleSetPtr> RuleSetMap;

//---------------------------- PROTOTYPES       -----------------------------//

