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
#pragma once

#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//
#include <map>
#include <set>

//---------------------------- USER INCLUDES    -----------------------------//
#include "UIExport.h"
#include "Singleton.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class IPopup;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: PopupManager
// 
// Managers all popup windows in the game.
//
// Derived From:
//     <Singleton>
//
// Project:
//     <Cities3D>
//
// Include:
//     PopupManager.h
//
class UI_EXPORT_SPEC PopupManager : public Singleton<PopupManager>
{
public:
	void AddPopup(const IPopup* popup, wxInt32 priority);
	void RemovePopup(const IPopup* popup);

	void AddWindow(wxWindow* window, wxInt32 priority);
	void RemoveWindow(wxWindow* window);

	void SetMainFrame(wxFrame* frame)
	{
		mMainFrame = frame;
	}

private:
	PopupManager();
	~PopupManager();

	void Poll();

	void OnTogglePopups(wxInt32);

	typedef std::map<const IPopup*, wxInt32> PopupMap;
	typedef std::map<wxInt32, PopupMap> PopupPriorityMap;
	PopupPriorityMap mPopupMap;

	typedef std::multimap<wxInt32, wxWindow*> WindowMap;
	WindowMap mWindowsToAvoid;

	wxFrame* mMainFrame;

	friend class Singleton<PopupManager>;
};

//---------------------------- PROTOTYPES       -----------------------------//

