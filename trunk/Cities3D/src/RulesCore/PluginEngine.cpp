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
#include "PluginEngine.h"
#include "DataObject.h"
#include "GameEngine.h"
#include "RuleEngine.h"
#include "GameObjectEngine.h"
#include "DrawObjectEngine.h"
#include "ImageEngine.h"
#include "UIEngine.h"
#include "OptionEngine.h"
#include "ImageSet.h"
#include "RuleSet.h"
#include "IDrawObject.h"
#include "Access.h"
#include "Controller.h"
#include "OptionDatabase.h"
#include "FilePath.h"
#include "KeyMapEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//function pointer for reading rulesets out of plugins
typedef const RuleSetMap &(*ProvideRuleSetPtr)();
typedef const OptionUIMap &(*ProvideOptionsPtr)();
typedef void (*ProvideModuleInfoPtr)(wxString&, wxInt32&);

//---------------------------- STATICS          -----------------------------//
namespace
{
	bool sFoundPlugins = false;
}

//---------------------------- PUBLIC           -----------------------------//
PluginEngine::PluginEngine(RuleEngine &engine, GameObjectEngine &game, 
						   DrawObjectEngine &draw, UIEngine& ui,
						   OptionEngine& option, KeyMapEngine& keymap)
: mRuleEngine(engine)
, mGameObjectEngine(game)
, mDrawObjectEngine(draw)
, mUIEngine(ui)
, mOptionEngine(option)
, mKeyMapEngine(keymap)
{
	Controller::get().AddReceiver(shEventLoadRulesets, 
		&PluginEngine::OnLoadRulesets, this);
}

PluginEngine::~PluginEngine()
{
	Controller::get().RemoveReceiver(shEventLoadRulesets, 
		&PluginEngine::OnLoadRulesets, this);
}

void PluginEngine::Start()
{
	// Only find plugins once at the start of the session.
	if(false == sFoundPlugins)
	{
		FindPlugins();
		sFoundPlugins = true;
	}
}

void PluginEngine::Stop()
{
	mLoadedRulesets.clear();
	
	//by destroying the objects, they are automatically unloaded from memory
	mRulePlugins.clear();
	mOptionPlugins.clear();
}

void PluginEngine::GetLoadedRuleSets(StringPairArray &array)
{
	//design by contract
	wxASSERT(true == array.empty());

	StringListPairArray::const_iterator it, itEnd = mLoadedRulesets.end();
	for(it = mLoadedRulesets.begin(); it != itEnd; ++it)
	{
		wxString module = it->first;
		
		StringList::const_iterator it2, it2End = it->second.end();
		for(it2 = it->second.begin(); it2 != it2End; ++it2)
		{
			array.push_back(StringPair(module, (*it2)));
		}
	}
}

bool PluginEngine::IsLoaded(const wxString &module)
{
	return (mRulePlugins.end() != mRulePlugins.find(module));
}

bool PluginEngine::IsLoaded(const wxString &module, const wxString &ruleset)
{
	//first see if the module is even loaded
	if(false == IsLoaded(module))
	{
		return false;
	}

	//search the string list for this module
	StringListPairArray::const_iterator it, itEnd(mLoadedRulesets.end());
	for(it = mLoadedRulesets.begin(); it != itEnd; ++it)
	{
		if(it->first == module)
		{
			const StringList &list(it->second);

			return (list.end() != std::find(list.begin(), list.end(), 
				ruleset));
		}
	}

	return false;
}

bool PluginEngine::LoadRuleset(const wxString &module, const wxString &ruleset)
{
	//sanity check
	if(true == IsLoaded(module, ruleset))
	{
		return true;
	}

	//look for the requested module
	StringMap::iterator it(mRulesetPaths.find(module));

	//see if we've even got that module
	if(mRulesetPaths.end() == it)
	{
		wxLogDebug(wxT("Unknown plugin: %s, for ruleset: %s"), module.c_str(), 
			ruleset.c_str());
		return false;
	}
	
	ProvideRuleSetPtr func = NULL;

	PluginMap::const_iterator itPlugins(mRulePlugins.find(it->first));

	//the next thing to do is to either load the module if it hasn't been 
	//loaded, or just find the reference to it if it has already been loaded
	if(mRulePlugins.end() == itPlugins)
	{
		//try and load the library
		DynamicLibraryPtr lib(new wxDynamicLibrary);
	
		//it hasn't been loaded, so try and load it
		if(false == lib->Load(it->second.c_str(), wxDL_NOW | wxDL_GLOBAL))
		{
			//failed for some reason
			wxLogError(wxString::Format(wxT("Could not load %s plugin (for ruleset) from disk."), 
				module.c_str()));
			return false;
		}

		//add this module handle to our list of open modules
		mRulePlugins.insert(std::make_pair(it->first, lib));

		//it's loaded now, so see if we can find the rule set provider
		func = (ProvideRuleSetPtr) (lib->GetSymbol(swProvideRulesets));
	}
	else
	{
		func = (ProvideRuleSetPtr) (itPlugins->second->GetSymbol(swProvideRulesets));
	}

	//shouldn't be in this list if it doesn't have one
	wxASSERT(NULL != func);

	//get the rule sets
	const RuleSetMap &rulesets(func());

	//see if this ruleset is in this map
	RuleSetMap::const_iterator itRuleSet;
	itRuleSet = rulesets.find(ruleset);

	//if it's there, time to lock and load
	if(rulesets.end() != itRuleSet)
	{
		LoadRulesetPrivate(itRuleSet->second, module, ruleset);

		return true;
	}

	wxLogDebug(wxT("Unknown ruleset: %s, in plugin: %s"), ruleset.c_str(), 
		module.c_str());
	return false;
}

bool PluginEngine::LoadOption(const wxString &module, const wxString &option)
{
	StringMap::iterator it(mOptionPaths.find(module));

	//see if we've even got that module
	if(mOptionPaths.end() == it)
	{
		wxLogDebug(wxT("Unknown plugin: %s, for option: %s"), module.c_str(), 
			option.c_str());
		return false;
	}

	ProvideOptionsPtr func = NULL;

	PluginMap::const_iterator itPlugins(mOptionPlugins.find(it->first));

	//the next thing to do is to either load the module if it hasn't been 
	//loaded, or just find the reference to it if it has already been loaded
	if(mOptionPlugins.end() == itPlugins)
	{
		//try and load the library
		DynamicLibraryPtr lib(new wxDynamicLibrary);
	
		//it hasn't been loaded, so try and load it
		if(false == lib->Load(it->second.c_str(), wxDL_NOW | wxDL_GLOBAL))
		{
			//failed for some reason
			wxLogError(wxString::Format(wxT("Could not load %s plugin (for option) from disk."),
				module.c_str()));
			return false;
		}

		//add this module handle to our list of open modules
		mOptionPlugins.insert(std::make_pair(it->first, lib));

		//it's loaded now, so see if we can find the rule set provider
		func = (ProvideOptionsPtr) (lib->GetSymbol(swProvideOptions));
	}
	else
	{
		func = (ProvideOptionsPtr) (itPlugins->second->GetSymbol(
			swProvideOptions));
	}

	//shouldn't be in this list if it doesn't have one
	wxASSERT(NULL != func);

	const OptionUIMap& options(func());

	//see if this ruleset is in this map
	OptionUIMap::const_iterator itOption;
	itOption = options.find(option);

	//if it's there, time to lock and load
	if(options.end() != itOption)
	{
		const OptionUIPtr& opt = itOption->second;

		mOptionEngine.LoadOption(module, option, opt);

		return true;
	}

	wxLogDebug(wxT("Unknown option: %s, in plugin: %s"), option.c_str(), 
		module.c_str());
	return false;
}

void PluginEngine::LoadBankData(std::vector<BankDataPtr> &array)
{
	//TODO: create a "loaded ruleset" iterator that abstracts all this madness

	//scan through each of the loaded rulesets, adding any bank data
	//to the passed in array
	StringListPairArray::const_iterator it, itEnd(mLoadedRulesets.end());
	for(it = mLoadedRulesets.begin(); it != itEnd; ++it)
	{
		//load up this set of rulesets so we can look through it
		PluginMap::const_iterator it2(mRulePlugins.find(it->first));
		wxASSERT(mRulePlugins.end() != it2);

		//grab the exported function pointer
		ProvideRuleSetPtr func = (ProvideRuleSetPtr) it2->second->GetSymbol(
			swProvideRulesets);
		wxASSERT(NULL != func);

		const RuleSetMap &rulesets(func());

		StringList::const_iterator it3, itEnd3(it->second.end());
		for(it3 = it->second.begin(); it3 != itEnd3; ++it3)
		{
			//find this ruleset name in the ruleset map
			RuleSetMap::const_iterator it4(rulesets.find(*it3));
			wxASSERT(rulesets.end() != it4);

			//and finally, copy any bank data over (whew!)
			std::copy(it4->second->bankdata_begin(), 
				it4->second->bankdata_end(), std::back_inserter(array));
		}
	}
}

void PluginEngine::LoadPlayerData(std::vector<PlayerDataPtr> &array)
{
	//TODO: create a "loaded ruleset" iterator that abstracts all this madness

	//scan through each of the loaded rulesets, adding any player data
	//to the passed in array
	StringListPairArray::const_iterator it, itEnd(mLoadedRulesets.end());
	for(it = mLoadedRulesets.begin(); it != itEnd; ++it)
	{
		//load up this set of rulesets so we can look through it
		PluginMap::const_iterator it2(mRulePlugins.find(it->first));
		wxASSERT(mRulePlugins.end() != it2);

		//grab the exported function pointer
		ProvideRuleSetPtr func = (ProvideRuleSetPtr) it2->second->GetSymbol(
			swProvideRulesets);
		wxASSERT(NULL != func);

		const RuleSetMap &rulesets(func());

		StringList::const_iterator it3, itEnd3(it->second.end());
		for(it3 = it->second.begin(); it3 != itEnd3; ++it3)
		{
			//find this ruleset name in the ruleset map
			RuleSetMap::const_iterator it4(rulesets.find(*it3));
			wxASSERT(rulesets.end() != it4);

			//and finally, copy any player data over (whew!)
			std::copy(it4->second->playerdata_begin(), 
				it4->second->playerdata_end(), std::back_inserter(array));
		}
	}
}

void PluginEngine::LoadBuildData(std::vector<BuildDataPtr> &array)
{
	//TODO: create a "loaded ruleset" iterator that abstracts all this madness

	//scan through each of the loaded rulesets, adding any player data
	//to the passed in array
	StringListPairArray::const_iterator it, itEnd(mLoadedRulesets.end());
	for(it = mLoadedRulesets.begin(); it != itEnd; ++it)
	{
		//load up this set of rulesets so we can look through it
		PluginMap::const_iterator it2(mRulePlugins.find(it->first));
		wxASSERT(mRulePlugins.end() != it2);

		//grab the exported function pointer
		ProvideRuleSetPtr func = (ProvideRuleSetPtr) it2->second->GetSymbol(
			swProvideRulesets);
		wxASSERT(NULL != func);

		const RuleSetMap &rulesets(func());

		StringList::const_iterator it3, itEnd3(it->second.end());
		for(it3 = it->second.begin(); it3 != itEnd3; ++it3)
		{
			//find this ruleset name in the ruleset map
			RuleSetMap::const_iterator it4(rulesets.find(*it3));
			wxASSERT(rulesets.end() != it4);

			//and finally, copy any build data over (whew!)
			std::copy(it4->second->builddata_begin(), 
				it4->second->builddata_end(), std::back_inserter(array));
		}
	}
}

void PluginEngine::LoadStats(std::vector<StatPtr> &array)
{
    //TODO: create a "loaded ruleset" iterator that abstracts all this madness

	//scan through each of the loaded rulesets, adding any player data
	//to the passed in array
	StringListPairArray::const_iterator it, itEnd(mLoadedRulesets.end());
	for(it = mLoadedRulesets.begin(); it != itEnd; ++it)
	{
		//load up this set of rulesets so we can look through it
		PluginMap::const_iterator it2(mRulePlugins.find(it->first));
		wxASSERT(mRulePlugins.end() != it2);

		//grab the exported function pointer
		ProvideRuleSetPtr func = (ProvideRuleSetPtr) it2->second->GetSymbol(
			swProvideRulesets);
		wxASSERT(NULL != func);

		const RuleSetMap &rulesets(func());

		StringList::const_iterator it3, itEnd3(it->second.end());
		for(it3 = it->second.begin(); it3 != itEnd3; ++it3)
		{
			//find this ruleset name in the ruleset map
			RuleSetMap::const_iterator it4(rulesets.find(*it3));
			wxASSERT(rulesets.end() != it4);

			//and finally, copy any build data over (whew!)
			std::copy(it4->second->stats().begin(), 
				it4->second->stats().end(), std::back_inserter(array));
		}
	}
}

const wxString& PluginEngine::GetModuleName(const wxString& module)
{
	StringMap::const_iterator it = mModuleNames.find(module);
	wxASSERT(mModuleNames.end() != it);

	return it->second;
}

wxInt32 PluginEngine::GetModulePriority(const wxString& module)
{
	PriorityMap::const_iterator it = mModulePriorities.find(module);
	wxASSERT(mModulePriorities.end() != it);

	return it->second;
}

const wxString& PluginEngine::GetRulesetName(const wxString& module,
	const wxString& ruleset)
{
	StringMap::const_iterator it = mRulesetNames.find(module + ruleset);
	wxASSERT(mRulesetNames.end() != it);

	return it->second;
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void PluginEngine::LoadRulesetPrivate(const RuleSetPtr &ruleset, 
									  const wxString &module, 
									  const wxString &name)
{
	wxLogDebug(wxT("Loading ruleset: %s, from plugin: %s"), name.c_str(), module.c_str());

	//set it in the array of loaded rulesets (kind of awkward, but it allows 
	//us to preserve load order)
	StringListPairArray::iterator it1, itEnd1(mLoadedRulesets.end());
	for(it1 = mLoadedRulesets.begin(); it1 != itEnd1; ++it1)
	{
		if(it1->first == module)
		{
			it1->second.push_back(name);
			break;
		}
	}

	if(it1 == itEnd1)
	{
		it1 = mLoadedRulesets.insert(itEnd1, std::make_pair(module, StringList()));
		it1->second.push_back(name);
	}

	//load these rules into the rule database
	IRuleEngine::RuleHash::const_iterator it2, itEnd2(ruleset->rules_end());
	for(it2 = ruleset->rules_begin(); it2 != itEnd2; ++it2)
	{
		mRuleEngine.mRuleHash[it2->first] = it2->second;
		wxLogDebug(wxT("Loaded rule: %s"), it2->first.cwx_str());
	}

	//load this logic into the logic database
	IRuleEngine::LogicHash::const_iterator it3, itEnd3(ruleset->logic_end());
	for(it3 = ruleset->logic_begin(); it3 != itEnd3; ++it3)
	{
		mRuleEngine.mLogicHash[it3->first] = it3->second;
		wxLogDebug(wxT("Loaded logic: %s"), it3->first.cwx_str());
	}

	//copy all rule extensions into the rule database
	IRuleEngine::MixinHash::const_iterator it4, itEnd4(ruleset->mixins_end());
	for(it4 = ruleset->mixins_begin(); it4 != itEnd4; ++it4)
	{
		IRuleEngine::StringHash::const_iterator it, itEnd(it4->second.end());
		for(it = it4->second.begin(); it != itEnd; ++it)
		{
			mRuleEngine.mMixinHash[it4->first].insert(*it);
		}
		wxLogDebug(wxT("Loaded extension: %s"), it4->first.cwx_str());
	}

	//load all branch logic into the database
	RuleSet::PriorityLogicHash::const_iterator it5, itEnd5(ruleset->branch_end());
	for(it5 = ruleset->branch_begin(); it5 != itEnd5; ++it5)
	{
		RuleSet::PriorityLogicList::const_iterator it, itEnd(it5->second.end());
		for(it = it5->second.begin(); it != itEnd; ++it)
		{
			mRuleEngine.mBranchHash[it5->first][it->first] = it->second;
			wxLogDebug(wxT("Loaded rule branch: %s %d"), it5->first.cwx_str(), it->first);
		}
	}

	//copy game objects into the game object database
	RuleSet::GameObjectHash::const_iterator it6, itEnd6(ruleset->gameobjects_end());
	for(it6 = ruleset->gameobjects_begin(); it6 != itEnd6; ++it6)
	{
		mGameObjectEngine.mObjectHash.insert((*it6));
		wxLogDebug(wxT("Loaded game object: %s"), it6->first.cwx_str());
	}

	//copy draw objects into the draw object database
    RuleSet::DrawObjectSet::const_iterator it8, itEnd8(ruleset->drawobjects_end());
    for(it8 = ruleset->drawobjects_begin(); it8 != itEnd8; ++it8)
    {
        mDrawObjectEngine.mObjectList.push_back((*it8));
		wxLogDebug(wxT("Loaded draw object: [%s]"), HashString(typeid(**it8).name()).cwx_str());
    }

	//copy image sets into the image database
	RuleSet::ImageSetList::const_iterator it7, it7End(ruleset->imagesets_end());
	for(it7 = ruleset->imagesets_begin(); it7 != it7End; ++it7)
	{
		IMAGE.AddImageSet(*it7);
		wxLogDebug(wxT("Loaded image set: %s"), (*it7)->file().c_str());
	}

	// Load UI plugins into the UI engine.
	const UIPluginPtr& ui = ruleset->ui_plugin();
	if(ui)
	{
		mUIEngine.AddPlugin(ui, module, name);
	}
}

void PluginEngine::FindPlugins()
{
	//search through the /plugins directory, looking for plugins with the
	//ProvideRuleSets functionality

	wxArrayString pluginList = FilePath::EnumPlugins();
	
	for(size_t i = 0; i < pluginList.GetCount(); ++i)
	{
		wxFileName filepath(pluginList[i]);
		wxString path(filepath.GetFullPath());
		//remove suffix (dll, so, sym, etc)
		wxString module(filepath.GetName());

		// Since we're loading a module, signal that the module's language 
		// locale should be added.
		Controller::get().Transmit(shEventAddCatalog, module);

		//now try loading the file, baby
		DynamicLibraryPtr lib(new wxDynamicLibrary);
		
		if(true == lib->Load(path, wxDL_NOW | wxDL_GLOBAL))
		{
			ProvideRuleSetPtr func;

			{
				//suppress error output because, for some reason, GetSymbol 
				//throws a wxLogError to the screen if it can't find the 
				//symbol, which it should not
				wxLogNull null;

				//now see if it has the right function call
				func = (ProvideRuleSetPtr) (lib->GetSymbol(swProvideRulesets));
			}

			//see if we've got it
			if(NULL != func)
			{
				mRulesetPaths[module] = path;
				wxLogDebug(wxT("Located plugin: %s"), module.c_str());

				// Load the rulesets and process each one in the option engine.
				const RuleSetMap &rulesets(func());

				// It also must have a module information function.
				ProvideModuleInfoPtr info;
				{
					wxLogNull null;
					info = (ProvideModuleInfoPtr) (lib->GetSymbol(swProvideModuleInfo));

					if(NULL != info)
					{
						wxString name;
						wxInt32 priority;
					
						info(name, priority);

						mModuleNames[module] = name;
						mModulePriorities[module] = priority;
					}
					else
					{
						wxLogError(wxT("Programmer Error: No ProvideModuleInfo ")
							wxT("in %s"), module.c_str());
					}
				}
				
				RuleSetMap::const_iterator it, itEnd = rulesets.end();
				for(it = rulesets.begin(); it != itEnd; ++it)
				{
					const wxString& name = it->first;
					const RuleSetPtr& ruleset = it->second;

					mUIEngine.ProcessRuleSet(module, name, ruleset);
					mKeyMapEngine.ProcessRuleSet(ruleset);

					const wxString& translation = ruleset->name();
					mRulesetNames[module + name] = translation;

                    static const wxString stFindingPlugins = _("Finding plugins... %s... found %s");
    
					wxString str = wxString::Format(stFindingPlugins.c_str(),
						module.c_str(), name.c_str());
					Controller::get().Transmit(shEventFindPlugin, str);

#ifndef _DEBUG
					wxMilliSleep(50);
#endif
				}
			}

			// Also look for option rulesets.
			ProvideOptionsPtr option;

			{
				wxLogNull null;

				//now see if it has the right function call
				option = (ProvideOptionsPtr) (lib->GetSymbol(swProvideOptions));
			}

			if(NULL != option)
			{
				mOptionPaths[module] = path;
				wxLogDebug(wxT("Located option: %s"), module.c_str());

				// Load the rulesets and process each one in the option engine.
				const OptionUIMap &options(option());
				
				OptionUIMap::const_iterator it, itEnd = options.end();
				for(it = options.begin(); it != itEnd; ++it)
				{
					const wxString& name = it->first;
					const OptionUIPtr& option = it->second;

					mOptionEngine.ProcessOption(module, name, option);
				}
			}
		}

		//DLL is automatically unloaded when the object goes out of scope and is 
		//destroyed
	}
}

void PluginEngine::OnLoadRulesets(const StringPairArray &array)
{
	//load all the rulesets in the game into memory
	StringPairArray::const_iterator it, itEnd = array.end();
	for(it = array.begin(); it != itEnd; ++it)
	{
		if(false == LoadRuleset(it->first, it->second))
		{
			wxLogError(wxString::Format(wxT("Unable to locate the ruleset %s ")
				wxT("in plugin %s"), it->second.c_str(), it->first.c_str()));
		}
	}	
}
