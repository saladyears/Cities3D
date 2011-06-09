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

#include "SettingsPane.h"
#include "IUIEngine.h"
#include "DataObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
class SettingsUI : public UIPlugin
{
public:
	SettingsUI()
	{
		GUI.RegisterMainMenuItem(MODULE, SETTINGS, swSettings, 0, 
			stSettings, shSettings, swSettings);
		GUI.RegisterGameMenuItem(MODULE, SETTINGS, swPanes, 0, 
			stSettingsAmp, swSettings, wxITEM_CHECK);
	}

	~SettingsUI()
	{
	}

	virtual void HandleMainMenuClick(const wxString& key)
	{
		wxASSERT(swSettings == key);
		
		wxSettingsPane* pane = new wxSettingsPane(GetMainFrame());

		Controller::get().Transmit(shEventChangeMainPane, 
			DataObject(static_cast<PaneMixin*>(pane), stSettings));
	}

	virtual void HandleGameMenuClick(const wxString& key, 
		wxAuiManager& manager, wxMenuItem* item)
	{
		wxASSERT(swSettings == key);
		wxASSERT(NULL != item);
		
		if(true == item->IsChecked())
		{
			wxSettingsPane* pane = new wxSettingsPane(GetMainFrame(), 
				item->GetId());

			// Add it as a popup window.
			manager.AddPane(pane, wxAuiPaneInfo().Caption(stSettings)
				.CloseButton(true).MaximizeButton(false).DestroyOnClose()
				.Float().Dockable(false).Name(swSettings));
		}
		else
		{
			wxAuiPaneInfo& pane = manager.GetPane(swSettings);
			manager.ClosePane(pane);
		}

		manager.Update();
	}
};

IMPLEMENT_UI_PLUGIN(SETTINGS, SettingsUI)

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
