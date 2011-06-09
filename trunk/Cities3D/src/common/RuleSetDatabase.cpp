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
#include "stdwx.h"
#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//
#include "RuleSetDatabase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	RuleSetMap::iterator findset(RuleSetMap &map, const wxString &name)
	{
		//first thing to do is see if this rule set exists
		RuleSetMap::iterator it(map.find(name));

		//if it does not exist, create the rule set
		if(map.end() == it)
		{
			map[name] = RuleSetPtr(new RuleSet(name));
			it = map.find(name);
		}
		
		return it;
	}

	struct DLL_LOCAL LocalSingletonGuard
	{
		~LocalSingletonGuard() 
		{
			if( (NULL != RuleSetDatabase::theLocalSingleton) &&
				(false == RuleSetDatabase::theLocalSingleton->map().empty()))
			{
				wxLogDebug(wxT("Deleting local singleton %s."),
					RuleSetDatabase::theLocalSingleton->map().begin()->first.c_str());
			}

			delete RuleSetDatabase::theLocalSingleton;
			RuleSetDatabase::theLocalSingleton = NULL;
		}
	};

	LocalSingletonGuard theLocalSingletonGuard;
}

//---------------------------- PUBLIC           -----------------------------//
void DLL_LOCAL RuleSetDatabase::RegisterName(const wxString& ruleset, 
								   const wxString& name)
{
	findset(mRuleSetMap, ruleset)->second->RegisterName(name);
}

void DLL_LOCAL RuleSetDatabase::RegisterRule(const wxString &ruleset, 
								   const RulePtr &rule, const HashString &name)
{
	//set the rule information (terrible looking code, I know)
	findset(mRuleSetMap, ruleset)->second->RegisterRule(rule, name);
}

void DLL_LOCAL RuleSetDatabase::RegisterLogic(const wxString &ruleset, 
									const LogicPtr &logic, 
									const HashString &name)
{
	//set the logic information
	findset(mRuleSetMap, ruleset)->second->RegisterLogic(logic, name);
}

void DLL_LOCAL RuleSetDatabase::RegisterMixin(const wxString &ruleset,
									const HashString &rule, 
									const HashString &key, 
									const HashString &extension)
{
	findset(mRuleSetMap, ruleset)->second->RegisterMixin(rule, key, extension);
}

void DLL_LOCAL RuleSetDatabase::RegisterBranch(const wxString &ruleset, 
									 const HashString &rule, 
									 const wxInt32 priority, 
									 const LogicPtr &logic)
{
	findset(mRuleSetMap, ruleset)->second->RegisterBranch(rule, priority, 
		logic);
}

void DLL_LOCAL RuleSetDatabase::RegisterGameObject(const wxString &ruleset, 
										 const HashString &name, 
										 const GameObjectPtr &object)
{
	findset(mRuleSetMap, ruleset)->second->RegisterGameObject(name, object);
}

void DLL_LOCAL RuleSetDatabase::RegisterDrawObject(const wxString &ruleset, 
										 const DrawObjectPtr &object)
{
	//set the object in the ruleset (terrible looking code, I know)
	findset(mRuleSetMap, ruleset)->second->RegisterDrawObject(object);
}

void DLL_LOCAL RuleSetDatabase::RegisterImageSet(const wxString &ruleset, 
									   const ImageSetPtr &imageset)
{
	findset(mRuleSetMap, ruleset)->second->RegisterImageSet(imageset);
}

void DLL_LOCAL RuleSetDatabase::RegisterBankData(const wxString &ruleset, 
									   const BankDataPtr &bankdata)
{
	findset(mRuleSetMap, ruleset)->second->RegisterBankData(bankdata);
}

void DLL_LOCAL RuleSetDatabase::RegisterPlayerData(const wxString &ruleset, 
										 const PlayerDataPtr &playerdata)
{
	findset(mRuleSetMap, ruleset)->second->RegisterPlayerData(playerdata);
}

void DLL_LOCAL RuleSetDatabase::RegisterBuildData(const wxString &ruleset, 
										const BuildDataPtr &builddata)
{
	findset(mRuleSetMap, ruleset)->second->RegisterBuildData(builddata);
}

void DLL_LOCAL RuleSetDatabase::RegisterUIPlugin(const wxString& ruleset, 
									   const UIPluginPtr& plugin)
{
	findset(mRuleSetMap, ruleset)->second->SetUIPlugin(plugin);
}

void DLL_LOCAL RuleSetDatabase::RegisterExclusions(const wxString& ruleset, 
										 const StringPairArray& e)
{
	findset(mRuleSetMap, ruleset)->second->RegisterExclusions(e);
}

void DLL_LOCAL RuleSetDatabase::RegisterKeyMap(const wxString& ruleset,
	const IKeyMapEngine::KeyMap& keymap)
{
	findset(mRuleSetMap, ruleset)->second->RegisterKeyMap(keymap);
}

void DLL_LOCAL RuleSetDatabase::RegisterStat(const wxString& ruleset, 
	const StatPtr& stat)
{
	findset(mRuleSetMap, ruleset)->second->RegisterStat(stat);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
