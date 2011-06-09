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
#include "LayoutManager.h"
#include "GameEngine.h"
#include "IPluginEngine.h"
#include "IUIEngine.h"
#include "FilePath.h"
#include "BaseListCtrl.h"
#include "ConfigDatabase.h"

//---------------------------- USER INCLUDES    -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	void CreateLayoutPath(wxString& path)
	{
		// Smash together the names of all loaded rulesets that contain UI.
		// We'll use that as the file name since it is guaranteed to be
		// unique.
		// Populate the html window.
		IPluginEngine::StringPairArray plugins;
		PLUGIN.GetLoadedRuleSets(plugins);

		// We only care about plugins that have UI elements.
		IPluginEngine::StringPairArray::const_iterator it, itEnd = plugins.end();
		for(it = plugins.begin(); it != itEnd; ++it)
		{
			const wxString& plugin = it->first;
			const wxString& ruleset = it->second;

			if(true == GUI.ProvidesUI(plugin, ruleset))
			{
				path += ruleset;
			}
		}
	}

    // The default layout strings.
    WX_DECLARE_STRING_HASH_MAP(wxString, LayoutMap);
    LayoutMap sDefaultLayouts;
}

//---------------------------- PUBLIC           -----------------------------//
void LayoutManager::LoadLayout(wxString& layout)
{
    // If our string set is empty, add all of the defaults.
    if(true == sDefaultLayouts.empty())
    {
        sDefaultLayouts[swCoreCities] = swCoreCitiesLayout;
        sDefaultLayouts[swCoreCitiesVolcano] = swCoreCitiesVolcanoLayout;
        sDefaultLayouts[swCoreDevCards] = swCoreDevCardsLayout;
        sDefaultLayouts[swCoreDevCardsVolcano] = swCoreDevCardsVolcanoLayout;
        sDefaultLayouts[swCoreNetworkCities] = swCoreNetworkCitiesLayout;
        sDefaultLayouts[swCoreNetworkCitiesVolcano] = swCoreNetworkCitiesVolcanoLayout;
        sDefaultLayouts[swCoreNetworkDevCards] = swCoreNetworkDevCardsLayout;
        sDefaultLayouts[swCoreNetworkDevCardsVolcano] = swCoreNetworkDevCardsVolcanoLayout;

        // Cities Online versions.
        sDefaultLayouts[swCoreCitiesOnlineCities] = swCoreCitiesOnlineCitiesLayout;
        sDefaultLayouts[swCoreCitiesOnlineCitiesVolcano] = swCoreCitiesOnlineCitiesVolcanoLayout;
        sDefaultLayouts[swCoreCitiesOnlineDevCards] = swCoreCitiesOnlineDevCardsLayout; 
        sDefaultLayouts[swCoreCitiesOnlineDevCardsVolcano] = swCoreCitiesOnlineDevCardsVolcanoLayout;
        sDefaultLayouts[swCoreCitiesOnlineNetworkCities] = swCoreCitiesOnlineNetworkCitiesLayout;
        sDefaultLayouts[swCoreCitiesOnlineNetworkCitiesVolcano] = swCoreCitiesOnlineNetworkCitiesVolcanoLayout;
        sDefaultLayouts[swCoreCitiesOnlineNetworkDevCards] = swCoreCitiesOnlineNetworkDevCardsLayout;
        sDefaultLayouts[swCoreCitiesOnlineNetworkDevCardsVolcano] = swCoreCitiesOnlineNetworkDevCardsVolcanoLayout;
    }

    wxString layoutPath;
    CreateLayoutPath(layoutPath);

    wxString defaultLayout;
    if(sDefaultLayouts.end() != sDefaultLayouts.find(layoutPath))
    {
        defaultLayout = sDefaultLayouts[layoutPath];
    }

    wxString path = swLayouts + swForwardSlash + layoutPath;

    layout = CONFIG.read<wxString>(swConfigSectionUI, path, defaultLayout);
}

void LayoutManager::SaveLayout(const wxString& layout)
{
    wxString layoutPath;
    CreateLayoutPath(layoutPath);

    wxString path = swLayouts + swForwardSlash + layoutPath;

    CONFIG.write<wxString>(swConfigSectionUI, path, layout);
}

void LayoutManager::RestoreLayout(wxString& layout)
{
    wxString layoutPath;
    CreateLayoutPath(layoutPath);

    if(sDefaultLayouts.end() != sDefaultLayouts.find(layoutPath))
    {
        layout = sDefaultLayouts[layoutPath];
    }

    wxString path = swLayouts + swForwardSlash + layoutPath;

    CONFIG.write<wxString>(swConfigSectionUI, path, layout);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
