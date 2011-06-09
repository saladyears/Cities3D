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
#include "UIEngine.h"
#include "IUIPlugin.h"
#include "GameEngine.h"
#include "OptionEngine.h"
#include "IPluginEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
UIEngine::UIEngine()
: mMainFrame(NULL)
, mMainMenuIndex(1)
, mGameMenuIndex(1)
, mQuitHandlerPriority(-1)
{
}

UIEngine::~UIEngine()
{
}

void UIEngine::Stop()
{
	mQuitHandler = QuitHandler();
	mQuitHandlerPriority = -1;
	mUIPluginMap.clear();
}

void UIEngine::SetMainFrame(wxWindow* window)
{
	wxASSERT(NULL != window);
	mMainFrame = window;
}

void UIEngine::CreateUI(const GamePtr& game, wxAuiManager& manager)
{
	UIPluginMap::iterator it, itEnd = mUIPluginMap.end();
	for(it = mUIPluginMap.begin(); it != itEnd; ++it)
	{
		it->second->CreateUI(game, manager);
	}
}

bool UIEngine::ProvidesUI(const wxString& plugin, 
	const wxString& ruleset) const
{
	bool ret = false;

	wxString key = plugin + ruleset;
	UIPluginMap::const_iterator it = mUIPluginMap.find(key);
	if(mUIPluginMap.end() != it)
	{
		UIPluginPtr ui = it->second;
		ret = ui->ProvidesUI();
	}

	return ret;
}

void UIEngine::SetGLCanvas(wxSettlersGLCanvas* canvas)
{
	UIPluginMap::iterator it, itEnd = mUIPluginMap.end();
	for(it = mUIPluginMap.begin(); it != itEnd; ++it)
	{
		it->second->SetGLCanvas(canvas);
	}
}

bool UIEngine::CanQuit(wxWindow* window)
{
	bool quit = true;

	if(mQuitHandler && (false == mQuitHandler(window)))
	{
		quit = false;
	}
	
	return quit;
}

void UIEngine::HandleMainMenuClick(size_t index)
{
	const MenuItem& item = mMainMenuIndexMap[index];

	wxASSERT(false == item.plugin.IsEmpty());

	// Ensure that the ruleset is loaded.
	PLUGIN.LoadRuleset(item.plugin, item.ruleset);

	// Tell the UI to handle it.
	wxString key = item.plugin + item.ruleset;
	UIPluginMap::const_iterator it = mUIPluginMap.find(key);
	wxASSERT(mUIPluginMap.end() != it);

	UIPluginPtr ui = it->second;
	ui->HandleMainMenuClick(item.key);
}

void UIEngine::HandleGameMenuClick(size_t index, wxAuiManager& manager,
	wxMenuItem* menuItem)
{
	const MenuItem& item = mGameMenuIndexMap[index];

	wxASSERT(false == item.plugin.IsEmpty());

	// Ensure that the ruleset is loaded.
	PLUGIN.LoadRuleset(item.plugin, item.ruleset);

	// Tell the UI to handle it.
	wxString key = item.plugin + item.ruleset;
	UIPluginMap::const_iterator it = mUIPluginMap.find(key);
	wxASSERT(mUIPluginMap.end() != it);

	UIPluginPtr ui = it->second;
	ui->HandleGameMenuClick(item.key, manager, menuItem);
}

void UIEngine::GetGameMenuItems(MenuItemMap& items) const
{
	// Handle stupidity.
	items.clear();

	// Grab all of the loaded rulesets and see if any of them have exclusions.
	// If any of their exclusions are in the game menu map, remove them.
	StringPairArray rulesets;
	StringPairArray exclusions;
	PLUGIN.GetLoadedRuleSets(rulesets);

	{
		// Create a list of exclusions.
		StringPairArray::const_iterator it, itEnd = rulesets.end();
		for(it = rulesets.begin(); it != itEnd; ++it)
		{
			const wxString& module = it->first;
			const wxString& name = it->second;

			InteractionHash::const_iterator itEx = mExclusions.find(module);
			if(mExclusions.end() != itEx)
			{
				const StringArrayHash& hash = itEx->second;
				StringArrayHash::const_iterator itSt = hash.find(name);
				if(hash.end() != itSt)
				{
					const StringPairArray& ex = itSt->second;
					std::copy(ex.begin(), ex.end(), 
						std::back_inserter(exclusions));
				}
			}
		}
	}

	MenuItemMap::const_iterator it, itEnd = mGameMenuItemMap.end();
	for(it = mGameMenuItemMap.begin(); it != itEnd; ++it)
	{
		const wxString& group = it->first;
		const MenuPriorityMap& map = it->second;

		MenuPriorityMap tempMap;

		// Copy over items, skipping exclusions.
		IUIEngine::MenuPriorityMap::const_iterator itMap, itMapEnd = map.end();
		for(itMap = map.begin(); itMap != itMapEnd; ++itMap)
		{
			const IUIEngine::MenuItem& item = itMap->second;
			const wxString& module = item.plugin;
			const wxString& name = item.ruleset;

			// If it's not in the exclusions, add it.
			if(exclusions.end() == std::find(exclusions.begin(), 
				exclusions.end(), std::make_pair(module, name)))
			{
				tempMap[itMap->first] = item;
			}
		}

		// If we get a category that's not empty, add it to the main map.
		if(false == tempMap.empty())
		{
			items[group] = tempMap;
		}
	}
}

void UIEngine::RegisterMainMenuItem(const wxString& plugin, 
	const wxString& ruleset, const wxString& category, wxInt32 priority,
	const wxString& text, const HashString& image, const wxString& key)
{
	MenuItem item;
	item.plugin = plugin;
	item.ruleset = ruleset;
	item.text = text;
	item.image = image;
	item.key = key;
	item.menutype = wxITEM_NORMAL;

	// Make sure this item does not already exist.
	MenuPriorityMap& items = mMainMenuItemMap[category];

	// See if this item has already been added.
	bool found = false;
	MenuPriorityMap::const_iterator it, itEnd = items.end();
	for(it = items.begin(); it != itEnd; ++it)
	{
		const MenuItem& testItem = it->second;
		if(testItem == item)
		{
			found = true;
			break;
		}
	}

	if(false == found)
	{
		wxASSERT(items.end() == items.find(priority));

		item.index = mMainMenuIndex;
		items[priority] = item;

		mMainMenuIndexMap[mMainMenuIndex] = item;

		wxLogDebug(wxT("Registering main menu item %s in group %s."),
			item.text.c_str(), item.key.c_str());

		++mMainMenuIndex;
	}
}

void UIEngine::RegisterGameMenuItem(const wxString& plugin, 
	const wxString& ruleset, const wxString& category, wxInt32 priority,
	const wxString& text, const wxString& key, wxItemKind menuType)
{
	MenuItem item;
	item.plugin = plugin;
	item.ruleset = ruleset;
	item.text = text;
	item.key = key;
	item.menutype = menuType;

	// Make sure this item does not already exist.
	MenuPriorityMap& items = mGameMenuItemMap[category];

	// See if this item has already been added.
	bool found = false;
	MenuPriorityMap::const_iterator it, itEnd = items.end();
	for(it = items.begin(); it != itEnd; ++it)
	{
		const MenuItem& testItem = it->second;
		if(testItem == item)
		{
			found = true;
			break;
		}
	}

	if(false == found)
	{
		wxASSERT(items.end() == items.find(priority));

		item.index = mGameMenuIndex;
		items[priority] = item;

		mGameMenuIndexMap[mGameMenuIndex] = item;

		++mGameMenuIndex;
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void UIEngine::AddPlugin(const UIPluginPtr& plugin, const wxString& module, 
	const wxString& ruleset)
{
	wxInt32 priority = 0;
	IUIPlugin::QuitHandler handler = plugin->GetQuitHandler(priority);
	if(handler && (priority > mQuitHandlerPriority))
	{
		mQuitHandlerPriority = priority;
		mQuitHandler = handler;
	}

	wxASSERT(NULL != mMainFrame);
	plugin->SetMainFrame(mMainFrame);

	mUIPluginMap[module + ruleset] = plugin;
}

void UIEngine::ProcessRuleSet(const wxString& module, const wxString& name, 
	const RuleSetPtr& ruleset)
{
	// Store all of the exclusions for each ruleset.
	const StringPairArray& exclusions = ruleset->exclusions();
	if(false == exclusions.empty())
	{
		mExclusions[module][name] = exclusions;
	}
}
