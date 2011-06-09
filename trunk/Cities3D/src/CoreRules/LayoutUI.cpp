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

#include "IUIEngine.h"
#include "DataObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
class LayoutUI : public UIPlugin
{
public:
	LayoutUI()
	{
		GUI.RegisterGameMenuItem(MODULE, LAYOUT, stLayoutAmp, 0, 
			stSetThisLayoutAsDefault, swSaveLayout);

        GUI.RegisterGameMenuItem(MODULE, LAYOUT, stLayoutAmp, 10, 
			stRestoreOriginalLayout, swRestoreLayout);
	}

	~LayoutUI()
	{
	}

	virtual void HandleGameMenuClick(const wxString& key, 
		wxAuiManager& manager, wxMenuItem*)
	{
        if(swSaveLayout == key)
        {
            Controller::get().Transmit(shEventSaveLayout, 0);
        }
        else if(swRestoreLayout == key)
        {
            Controller::get().Transmit(shEventRestoreLayout, 0);
        }
        else
        {
            wxASSERT(false);
        }

	}
};

IMPLEMENT_UI_PLUGIN(LAYOUT, LayoutUI)

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
