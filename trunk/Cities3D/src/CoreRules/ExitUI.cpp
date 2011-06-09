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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
class ExitUI : public UIPlugin
{
public:
	ExitUI()
	{
		GUI.RegisterMainMenuItem(MODULE, EXIT, swExit, 0, stExit, 
			shExit, swExit);
		GUI.RegisterGameMenuItem(MODULE, EXIT, swFileAmp, 1000, stExitAmp, 
			swExit);
	}

	~ExitUI()
	{
	}

	virtual void HandleMainMenuClick(const wxString& key)
	{
		wxASSERT(swExit == key);
		
		Controller::get().Transmit(shEventExit, GetMainFrame());
	}

	virtual void HandleGameMenuClick(const wxString& key, wxAuiManager&,
		wxMenuItem*)
	{
		wxASSERT(swExit == key);
		
		Controller::get().Transmit(shEventExit, GetMainFrame());
	}
};

IMPLEMENT_UI_PLUGIN(EXIT, ExitUI)

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
