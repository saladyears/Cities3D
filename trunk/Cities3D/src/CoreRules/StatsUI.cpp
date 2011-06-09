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
#include "UIPlugin.h"
#include "RuleSetDatabase.h"
#include "Controller.h"
#include "GameEngine.h"
#include "DataObject.h"
#include "IUIEngine.h"
#include "StatsPane.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
class StatsUI : public UIPlugin
{
public:
	StatsUI()
	{
		GUI.RegisterGameMenuItem(MODULE, STATS, swPanes, 200, stStatsAmp, 
			swStats, wxITEM_CHECK);
	}

	~StatsUI()
	{
	}

	virtual void HandleGameMenuClick(const wxString& key, wxAuiManager& manager,
		wxMenuItem* item)
	{
		wxASSERT(swStats == key);
		wxASSERT(NULL != item);
		
		if(true == item->IsChecked())
		{
			wxStatsPane* pane = new wxStatsPane(GetMainFrame(), 
				item->GetId());

			// Add it as a popup window.
			manager.AddPane(pane, wxAuiPaneInfo().Caption(stStats)
				.CloseButton(true).MaximizeButton(false).DestroyOnClose()
				.Float().Dockable(false).Name(swStats));
		}
		else
		{
			wxAuiPaneInfo& pane = manager.GetPane(swStats);
			manager.ClosePane(pane);
		}

		manager.Update();

		Controller::get().Transmit(shEventStats, 0);
	}
};

IMPLEMENT_UI_PLUGIN(STATS, StatsUI)

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
