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

#include "EruptionUI.h"
#include "DataObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
class VolcanoUI : public UIPlugin
{
public:
	VolcanoUI()
	{
	}

	~VolcanoUI()
	{
	}

	virtual bool ProvidesUI() const
	{
		return true;
	}

	virtual void CreateUI(const GamePtr& game, wxAuiManager& manager)
	{
		wxWindow* frame = GetMainFrame();

		wxEruptionUI* volcano = new wxEruptionUI(frame, game);
		
		// Add it to the side pane.
		Controller::get().Transmit(shEventAddSidePane, 
			DataObject(static_cast<wxWindow*>(volcano), stVolcano));
	}
};

IMPLEMENT_UI_PLUGIN(VOLCANO, VolcanoUI)

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
