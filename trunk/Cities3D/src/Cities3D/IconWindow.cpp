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
#include "IconWindow.h"
#include "GameEngine.h"
#include "ContextListCtrl.h"
#include "IUIEngine.h"
#include "SkinManager.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	wxContextListCtrl* CreateIconGroup(wxWindow* window, 
		const IUIEngine::MenuItemMap& items, const wxString& type)
	{
		wxContextListCtrl* group = NULL;

		IUIEngine::MenuItemMap::const_iterator it = items.find(type);
		if(items.end() != it)
		{
			const IUIEngine::MenuPriorityMap& items = it->second;

			group = new wxContextListCtrl(window);
			group->Lock();

			IUIEngine::MenuPriorityMap::const_iterator it, itEnd = items.end();
			for(it = items.begin(); it != itEnd; ++it)
			{
				const IUIEngine::MenuItem& item = it->second;
				group->AddMenuItem(item.text, item.index, item.image);

				wxLogDebug(wxT("Adding menu item %s to group %s."), 
					item.text.c_str(), type.c_str());
			}

			group->Unlock();
		}

		return group;
	}
}

//---------------------------- PUBLIC           -----------------------------//
wxIconWindow::wxIconWindow(wxWindow* parent)
: wxPanel(parent, wxID_ANY)
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer* horz = new wxBoxSizer(wxHORIZONTAL);
	wxSizer* vert = new wxBoxSizer(wxVERTICAL);

	SetBackgroundColour(SKIN.Element(shBackground));

	// The main menu is a strip of icons down the left side.  This may get more
	// flexible in the future, but it probably won't.
	const IUIEngine::MenuItemMap& menuItems = GUI.GetMainMenuItems();

	wxContextListCtrl* play = CreateIconGroup(this, menuItems, swPlay);
	wxContextListCtrl* settings = CreateIconGroup(this, menuItems, swSettings);
	wxContextListCtrl* actions = CreateIconGroup(this, menuItems, swActions);
	wxContextListCtrl* editors = CreateIconGroup(this, menuItems, swEditor);
	wxContextListCtrl* exit = CreateIconGroup(this, menuItems, swExit);

	if(play)
	{
		vert->Add(play);
		vert->AddSpacer(50);
	}

	if(settings)
	{
		vert->Add(settings);
		vert->AddSpacer(50);
	}

	if(actions)
	{
		vert->Add(actions);
		vert->AddSpacer(50);
	}

	if(editors)
	{
		vert->Add(editors);
	}

	vert->AddStretchSpacer();

	if(exit)
	{
		vert->Add(exit);
	}

	wxStaticText *text = new wxStaticText(this, -1, swCitiesOnline + swSpace +
		GAMEVERSION);
	text->SetBackgroundColour(this->GetBackgroundColour());
	text->SetForegroundColour(wxColour(64, 64, 64));
	vert->Add(text, 0, wxEXPAND | wxTOP, 4);

	horz->Add(vert, 1, wxEXPAND | wxALL, 4);
	sizer->Add(horz, 1, wxEXPAND | wxTOP | wxBOTTOM, 4);
	SetSizer(sizer);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
