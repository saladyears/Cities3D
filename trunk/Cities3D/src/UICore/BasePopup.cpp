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
#include "BasePopup.h"
#include "PopupManager.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	// The delay before showing a popup.
	const wxInt32 DELAY_TICKS = 3;
}

//---------------------------- PUBLIC           -----------------------------//
bool BasePopup::TestPopup(const wxPoint& pos, wxInt32& delay) const
{
	bool ret = false;

	if(true == mThis->IsShown())
	{
		wxPoint point = mThis->ScreenToClient(pos);

		wxRect rect = mThis->GetSize();
		if(true == rect.Contains(point))
		{
			ret = true;
		}
	}

	delay = DELAY_TICKS;

	// See if it's within the window rect.
	return ret;
}

//---------------------------- PROTECTED        -----------------------------//
BasePopup::BasePopup(wxWindow* thisWindow)
: mThis(thisWindow)
{
	// TODO: Add a priority that gets passed in.
	PopupManager::get().AddPopup(this, 2);
}

BasePopup::~BasePopup()
{
	PopupManager::get().RemovePopup(this);
}

//---------------------------- PRIVATE          -----------------------------//
