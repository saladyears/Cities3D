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
#include "OptionEngine.h"
#include "GameEngine.h"
#include "IOptionUI.h"
#include "DataObject.h"
#include "Controller.h"
#include "IPluginEngine.h"
#include "IRuleEngine.h"
#include "Map.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
OptionEngine::OptionEngine()
: mSkipEnforcement(false)
{
	Controller::get().AddReceiver(shEventLoadOptions, 
		&OptionEngine::OnLoadOptions, this);
	Controller::get().AddReceiver(shEventLoadMap, 
		&OptionEngine::OnLoadMap, this);
}

OptionEngine::~OptionEngine()
{
	Controller::get().RemoveReceiver(shEventLoadOptions, 
		&OptionEngine::OnLoadOptions, this);
	Controller::get().RemoveReceiver(shEventLoadMap, 
		&OptionEngine::OnLoadMap, this);
}

void OptionEngine::Stop()
{
	mLoadedOptionMap.clear();
	mSelectedOptions.clear();
	mSelectedExclusions.clear();
	mLoadedRulesets.clear();
}

void OptionEngine::GetOptions(RulesetPriorityMap& options) const
{
	RulesetPriorityMap::const_iterator it, itEnd = mOptionMap.end();
	RulesetOptionMap temp;

	wxInt32 priority = -1;

	// Go through each subtree of the option map.  If any of the options are
	// not excluded, copy them into the outgoing priority map.
	for(it = mOptionMap.begin(); it != itEnd; ++it)
	{
		priority = it->first;
		const RulesetOptionMap& map = it->second;

		RulesetOptionMap::const_iterator itMap, itMapEnd = map.end();
		for(itMap = map.begin(); itMap != itMapEnd; ++itMap)
		{
			const wxString& module = itMap->first;
			const OptionPriorityMap& opt = itMap->second;

			OptionPriorityMap::const_iterator itOpt, itOptEnd = opt.end();
			for(itOpt = opt.begin(); itOpt != itOptEnd; ++itOpt)
			{
				wxInt32 optionPriority = itOpt->first;
				const wxString& ruleset = itOpt->second;
	
				if(false == IsExcluded(module, ruleset))
				{
					temp[module][optionPriority] = ruleset;
				}
			}
		}

		if(false == temp.empty())
		{
			options[priority] = it->second;
			temp.clear();
		}
	}
}

void OptionEngine::GetSelectedOptions(StringPairArray& options) const
{
	StringPriorityMap::const_iterator it, itEnd = mSelectedOptions.end();
	for(it = mSelectedOptions.begin(); it != itEnd; ++it)
	{
		const StringPairArray& array = it->second;
		std::copy(array.begin(), array.end(), std::back_inserter(options));
	}
}

void OptionEngine::SetSelectedOptions(const StringPairArray& options)
{
	// These need to be added in their priority buckets.
	StringPairArray::const_iterator it, itEnd = options.end();
	for(it = options.begin(); it != itEnd; ++it)
	{
		const wxString& module = it->first;
		wxInt32 priority = PLUGIN.GetModulePriority(module);

		mSelectedOptions[priority].push_back((*it));
	}
	
	BuildSelectedExclusions();
}

bool OptionEngine::HasSelectedOptions() const
{
	bool ret = false;

	StringPriorityMap::const_iterator it, itEnd = mSelectedOptions.end();
	for(it = mSelectedOptions.begin(); it != itEnd; ++it)
	{
		const StringPairArray& array = it->second;
		if(false == array.empty())
		{
			ret = true;
			break;
		}
	}

	return ret;
}

void OptionEngine::GetOptionStrings(StringArray& pieces) const
{
	StringPriorityMap::const_iterator it, itEnd = mSelectedOptions.end();
	for(it = mSelectedOptions.begin(); it != itEnd; ++it)
	{
		const StringPairArray& array = it->second;
		StringPairArray::const_iterator itAr, itArEnd = array.end();
		for(itAr = array.begin(); itAr != itArEnd; ++itAr)
		{
			const wxString& module = itAr->first;
			const wxString& ruleset = itAr->second;

			OptionUIPtr option = GetOptionUI(module, ruleset);
			
            if(option)
            {
			    pieces.push_back(option->short_description());
            }
		}
	}
}

OptionUIPtr OptionEngine::GetOptionUI(const wxString& module,
	const wxString& ruleset) const
{
	// Make sure the ruleset is loaded so we have the option in memory.
	PLUGIN.LoadOption(module, ruleset);

	wxString str = module + ruleset;
	LoadedOptionMap::const_iterator it = mLoadedOptionMap.find(str);

    OptionUIPtr ret;
    if(mLoadedOptionMap.end() != it)
    {
        ret = it->second;
    }

	return ret;
}

bool OptionEngine::IsSelected(const wxString& module, 
	const wxString& ruleset) const
{
	StringPair option(module, ruleset);

	wxInt32 priority = PLUGIN.GetModulePriority(module);
	const StringPairArray& array = mSelectedOptions[priority];

	StringPairArray::const_iterator it = std::find(array.begin(),
		array.end(), option);

	bool ret = true;
	if(array.end() == it)
	{
		ret = false;
	}

	return ret;
}

bool OptionEngine::IsExcluded(const wxString& module, 
	const wxString& ruleset) const
{
	wxString str = module + ruleset;

	StringSet::const_iterator it = mSelectedExclusions.find(str);

	bool ret = true;
	if(mSelectedExclusions.end() == it)
	{
		ret = false;
	}

	// Check dependencies if it is not explicitly excluded.
	if(false == ret)
	{
		InteractionHash::const_iterator itEx = mDependencies.find(module);
		if(mDependencies.end() != itEx)
		{
			const StringArrayHash& hash = itEx->second;
			StringArrayHash::const_iterator itSt = hash.find(ruleset);
			if(hash.end() != itSt)
			{
				const StringPairArray& ex = itSt->second;

				StringPairArray::const_iterator itAr, itArEnd = ex.end();
				for(itAr = ex.begin(); itAr != itArEnd; ++itAr)
				{
					wxString module = itAr->first;
					const wxString& ruleset = itAr->second;

					// If the dependency is not in the set of map and
					// option rulesets, we can't use it.
					if(mLoadedRulesets.end() == 
						mLoadedRulesets.find(module + ruleset))
					{
						ret = true;
						break;
					}
				}
			}
		}
	}

	return ret;
}

void OptionEngine::ClearOptions()
{
	mSelectedOptions.clear();

    static const wxString stAllOptionsHaveBeenCleared = _("All options have been cleared.");
	RULE.Execute(shRuleGenerateSystemMessage, DataObject(stAllOptionsHaveBeenCleared));

	BuildSelectedExclusions();
	BuildLoadedRulesets();
}

void OptionEngine::SelectOption(const wxString& module, 
	const wxString& ruleset)
{
	StringPair option(module, ruleset);

	wxInt32 priority = PLUGIN.GetModulePriority(module);
	StringPairArray& array = mSelectedOptions[priority];

	if(array.end() == std::find(array.begin(), array.end(), option))
	{
		array.push_back(option);
		
        static const wxString stTheOptionIsNowSelected = _("The option %s is now selected.");
    
		// Fire off the system message.
		const wxString& name = PLUGIN.GetRulesetName(module, ruleset);
		wxString str = wxString::Format(stTheOptionIsNowSelected.c_str(),
			name.c_str());
		RULE.Execute(shRuleGenerateSystemMessage, DataObject(str));

		BuildSelectedExclusions();
		BuildLoadedRulesets();
	}
}

void OptionEngine::UnselectOption(const wxString& module, 
	const wxString& ruleset)
{
	StringPair option(module, ruleset);

	wxInt32 priority = PLUGIN.GetModulePriority(module);
	StringPairArray& array = mSelectedOptions[priority];

	StringPairArray::iterator it = std::find(array.begin(), array.end(), 
		option);

	if(array.end() != it)
	{
		array.erase(it);
		BuildSelectedExclusions();
		BuildLoadedRulesets();

		// Fire off the system message.
        static const wxString stTheOptionIsNoLongerSelected = _("The option %s is no longer selected.");
    
		const wxString& name = PLUGIN.GetRulesetName(module, ruleset);
		wxString str = wxString::Format(stTheOptionIsNoLongerSelected.c_str(), name.c_str());
		RULE.Execute(shRuleGenerateSystemMessage, DataObject(str));

		if(false == mSkipEnforcement)
		{
			EnforceInteractions();
		}
	}
}

void OptionEngine::UpdateOption(const wxString& option, const wxString& key,
	const wxString& value)
{
	LoadedOptionMap::const_iterator it = mLoadedOptionMap.find(option);
	wxASSERT(mLoadedOptionMap.end() != it);

	it->second->UpdateUI(key, value);
}

void OptionEngine::LoadOptions() const
{
	// Before loading the options, examine them to see if any silent 
	// dependencies need to be added to the list as well.
	StringPriorityMap silent_dependencies;

	StringPriorityMap::const_iterator it, itEnd = mSelectedOptions.end();
	for(it = mSelectedOptions.begin(); it != itEnd; ++it)
	{
		const StringPairArray& array = it->second;
		StringPairArray::const_iterator itAr, itArEnd = array.end();
		for(itAr = array.begin(); itAr != itArEnd; ++itAr)
		{
			wxString module = itAr->first;
			const wxString& ruleset = itAr->second;

			// If there are any silent dependencies for the selected option,
			// add them to the list.
			InteractionHash::const_iterator itI = 
				mSilentDependencies.find(module);
			if(mSilentDependencies.end() != itI)
			{
				const StringArrayHash& hash = itI->second;
				StringArrayHash::const_iterator itH = hash.find(ruleset);
				if(itH != hash.end())
				{
					// Found dependencies, add them to the list.
					const StringPairArray& array = itH->second;

					StringPairArray::const_iterator itA, itAEnd = array.end();
					for(itA = array.begin(); itA != itAEnd; ++itA)
					{
						wxString moduleDep = itA->first;
						const wxString& optionDep = itA->second;

						wxInt32 priority = 
							PLUGIN.GetModulePriority(moduleDep);

						silent_dependencies[priority].push_back(
							std::make_pair(moduleDep, optionDep));
					}
				}
			}
		}
	}

	// Add any silent dependencies to the selected options.
	itEnd = silent_dependencies.end();
	for(it = silent_dependencies.begin(); it != itEnd; ++it)
	{
		wxInt32 priority = it->first;
		const StringPairArray& src = it->second;
		StringPairArray& dest = mSelectedOptions[priority];

		std::copy(src.begin(), src.end(), std::back_inserter(dest));
	}
	

	// Load all explicitly selected options.
	itEnd = mSelectedOptions.end();
	for(it = mSelectedOptions.begin(); it != itEnd; ++it)
	{
		const StringPairArray& array = it->second;
		StringPairArray::const_iterator itAr, itArEnd = array.end();
		for(itAr = array.begin(); itAr != itArEnd; ++itAr)
		{
			wxString module = itAr->first;
			const wxString& ruleset = itAr->second;

			PLUGIN.LoadRuleset(module, ruleset);
		}
	}

	// Load all silent options.
	LoadSilentOptions();
}

void OptionEngine::SaveOptionsToFile(const wxString& path) const
{
	// Create an XML file and store each currently selected option in the
	// file.
	wxXmlDocument doc;
	wxXmlNode *root = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, swOptionSet);
	
	// Go through each loaded option and store it, in order by priority.
	StringPriorityMap::const_iterator it, itEnd = mSelectedOptions.end();
	for(it = mSelectedOptions.begin(); it != itEnd; ++it)
	{
		const StringPairArray& array = it->second;
		StringPairArray::const_iterator itAr, itArEnd = array.end();
		for(itAr = array.begin(); itAr != itArEnd; ++itAr)
		{
			wxString module = itAr->first;
			const wxString& option = itAr->second;

			// See if this module has a UI where it will need to write out its
			// own values.
			wxString str = module + option;
			LoadedOptionMap::const_iterator it = mLoadedOptionMap.find(str);
			wxASSERT(mLoadedOptionMap.end() != it);

			const OptionUIPtr& optionUI = it->second;

			wxXmlNode* node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
				swOption);
			node->AddProperty(swModule, module);
			node->AddProperty(swOption, option);

			if(true == optionUI->HasUI())
			{
				optionUI->SaveToXML(node);
			}

			root->AddChild(node);
		}
	}

	doc.SetRoot(root);
	doc.Save(path);
}

void OptionEngine::LoadOptionsFromFile(const wxString& path)
{
	// This is a kind of crazy mechanism of turning the whole XML file into a
	// string and sending that along to be parsed.
	wxFile file(path);
	if(true == file.IsOpened())
	{
		size_t size = file.Length();

		wxString memString;
		{
			wxStringBuffer buffer(memString, size);
			file.Read(buffer, size);
		}

		RULE.Execute(shRuleLoadOptionsFromFile, DataObject(memString));

		file.Close();
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void OptionEngine::ProcessOption(const wxString& module, const wxString& name,
	const OptionUIPtr& option)
{
	wxASSERT(option);

	bool silent = option->is_silent();
	wxInt32 priority = PLUGIN.GetModulePriority(module);
	wxInt32 optionPriority = option->priority();

	// Silent options are not publically available.
	if(false == silent)
	{
		mOptionMap[priority][module][optionPriority] = name;
	}
	else
	{
		mSilentOptionMap[priority][module][optionPriority] = name;
	}
	
	// Store off all the exclusions this option has.
	StringPairArray exclusions;
	option->exclusions(exclusions);

	if(false == exclusions.empty())
	{
		if(true == silent)
		{
			mSilentOptionExclusions[module][name] = exclusions;
		}
		else
		{
			StringPairArray& ex = mExclusions[module][name];

			std::copy(exclusions.begin(), exclusions.end(), 
				std::back_inserter(ex));

			// In addition, mark all exclusions this option has as excluding
			// this option.  This way, earlier rulesets pick up exclusions of
			// later rulesets.
			StringPairArray::const_iterator it, itEnd = exclusions.end();
			for(it = exclusions.begin(); it != itEnd; ++it)
			{
				wxString exModule = it->first;
				const wxString& exOption = it->second;

				mExclusions[exModule][exOption].push_back(
					std::make_pair(module, name));

				mSilentOptionExclusions[exModule][exOption].push_back(
					std::make_pair(module, name));
			}
		}
	}

	// Store off dependencies as well
	StringPairArray dependencies;
	option->dependencies(dependencies);

	if(false == dependencies.empty())
	{
		if(true == silent)
		{
			mSilentOptionDependencies[module][name] = dependencies;
		}
		else
		{
			mDependencies[module][name] = dependencies;
		}
	} 
	else if(true == silent)
	{
		wxLogDebug(wxT("Option %s in %s is silent but has no dependencies!"),
			name.c_str(), module.c_str());
		wxASSERT_MSG(false, wxT("Silent option without dependencies!"));
	}

	StringPairArray silent_dependencies;
	option->silent_dependencies(silent_dependencies);

	if(false == silent_dependencies.empty())
	{
		mSilentDependencies[module][name] = silent_dependencies;
	}
}

void OptionEngine::LoadOption(const wxString& module, const wxString& name,
	const OptionUIPtr& option)
{
	wxASSERT(option);

	wxString str = module + name;
	mLoadedOptionMap[str] = option;
}

void OptionEngine::OnLoadOptions(const StringPairArray& options)
{
	SetSelectedOptions(options);
}

void OptionEngine::OnLoadMap(wxInt32)
{
	BuildLoadedRulesets();
	EnforceInteractions();
}

void OptionEngine::BuildSelectedExclusions()
{
	mSelectedExclusions.clear();

	StringPriorityMap::const_iterator it, itEnd = mSelectedOptions.end();
	for(it = mSelectedOptions.begin(); it != itEnd; ++it)
	{
		const StringPairArray& array = it->second;
		StringPairArray::const_iterator itA, itAEnd = array.end();
		for(itA = array.begin(); itA != itAEnd; ++itA)
		{
			const wxString& module = itA->first;
			const wxString& name = itA->second;

			// See if we have any exclusions for this ruleset.  If so, add them
			// to the exclusion set.
			InteractionHash::iterator itEx = mExclusions.find(module);
			if(mExclusions.end() != itEx)
			{
				const StringArrayHash& hash = itEx->second;
				StringArrayHash::const_iterator itSt = hash.find(name);
				if(hash.end() != itSt)
				{
					const StringPairArray& ex = itSt->second;

					StringPairArray::const_iterator itAr, itArEnd = ex.end();
					for(itAr = ex.begin(); itAr != itArEnd; ++itAr)
					{
						wxString str = itAr->first + itAr->second;
						mSelectedExclusions.insert(str);
					}
				}
			}
		}
	}
}

void OptionEngine::BuildLoadedRulesets()
{
	// Prepare the set of loaded map and option rulesets.
	mLoadedRulesets.clear();
	{
		const Map::StringPairArray& rulesets = MAP.rulesets();
		Map::StringPairArray::const_iterator it, itEnd = rulesets.end();
		for(it = rulesets.begin(); it != itEnd; ++it)
		{
			wxString module = it->first;
			const wxString& ruleset = it->second;

			mLoadedRulesets.insert(module + ruleset);
		}
	}

	// Also add all options currently selected.
	{
		StringPriorityMap::const_iterator it, itEnd = mSelectedOptions.end();
		for(it = mSelectedOptions.begin(); it != itEnd; ++it)
		{
			const StringPairArray& array = it->second;
			StringPairArray::const_iterator itAr, itArEnd = array.end();
			for(itAr = array.begin(); itAr != itArEnd; ++itAr)
			{
				const wxString& module = itAr->first;
				const wxString& option = itAr->second;

				mLoadedRulesets.insert(module + option);
			}
		}
	}
}

void OptionEngine::EnforceInteractions()
{
	// Go through each of the currently loaded options, find all the ones
	// that are excluded, and unselect them.
	StringPairArray nowExcluded;

	StringPriorityMap::const_iterator it, itEnd = mSelectedOptions.end();
	for(it = mSelectedOptions.begin(); it != itEnd; ++it)
	{
		const StringPairArray& array = it->second;
		StringPairArray::const_iterator itAr, itArEnd = array.end();
		for(itAr = array.begin(); itAr != itArEnd; ++itAr)
		{
			const wxString& module = itAr->first;
			const wxString& ruleset = itAr->second;

			if(true == IsExcluded(module, ruleset))
			{
				nowExcluded.push_back(*itAr);
			}
		}
	}

	// Now, remove them all.
	mSkipEnforcement = true;

	StringPairArray::const_iterator itEx, itExEnd = nowExcluded.end();
	for(itEx = nowExcluded.begin(); itEx != itExEnd; ++itEx)
	{
		const wxString& module = itEx->first;
		const wxString& ruleset = itEx->second;

		UnselectOption(module, ruleset);
	}
	mSkipEnforcement = false;
}

void OptionEngine::LoadSilentOptions() const
{
	// Go through all silent options and check their exclusions.  If their
	// exclusions aren't loaded, check their dependencies.  If all their
	// dependencies are loaded, load the option.
	RulesetPriorityMap::const_iterator it, itEnd = mSilentOptionMap.end();
	for(it = mSilentOptionMap.begin(); it != itEnd; ++it)
	{
		const RulesetOptionMap& map = it->second;
		RulesetOptionMap::const_iterator itMap, itMapEnd = map.end();
		for(itMap = map.begin(); itMap != itMapEnd; ++itMap)
		{
			const wxString& module = itMap->first;
			const OptionPriorityMap& opts = itMap->second;

			OptionPriorityMap::const_iterator itOpts, itOptsEnd = opts.end();
			for(itOpts = opts.begin(); itOpts != itOptsEnd; ++itOpts)
			{
				bool exclusions = true;
				const wxString& option = itOpts->second;
			
				// Check the exclusions for this option.
				InteractionHash::const_iterator itMo =
					mSilentOptionExclusions.find(module);
				if(mSilentOptionExclusions.end() != itMo)
				{
					const StringArrayHash& hash = itMo->second;
					StringArrayHash::const_iterator itOpt = hash.find(option);
					if(hash.end() != itOpt)
					{
						const StringPairArray& array = itOpt->second;
						StringPairArray::const_iterator itEx, 
							itExEnd = array.end();
						for(itEx = array.begin(); itEx != itExEnd; ++itEx)
						{
							if(true == PLUGIN.IsLoaded(itEx->first, 
								itEx->second))
							{
								exclusions = false;
								break;
							}
						}
					}
				}

				// If this option is excluded, try the next one.
				if(false == exclusions)
				{
					continue;
				}

				bool dependencies = true;

				// It isn't excluded, so check if all its dependencies are loaded.
				itMo = mSilentOptionDependencies.find(module);
				wxASSERT(mSilentOptionDependencies.end() != itMo);

				const StringArrayHash& hash = itMo->second;
				StringArrayHash::const_iterator itOpt = hash.find(option);
				wxASSERT(hash.end() != itOpt);

				const StringPairArray& array = itOpt->second;
				StringPairArray::const_iterator itEx, itExEnd = array.end();
				for(itEx = array.begin(); itEx != itExEnd; ++itEx)
				{
					if(false == PLUGIN.IsLoaded(itEx->first, itEx->second))
					{
						dependencies = false;
						break;
					}
				}

				// If all the dependencies are loaded, load this silent option.
				if(true == dependencies)
				{
					wxString thisModule = module;
					PLUGIN.LoadRuleset(thisModule, option);
				}
			}
		}
	}
}
