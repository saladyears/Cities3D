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
#include "OptionWindow.h"
#include "ConfigDatabase.h"
#include "SkinManager.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
wxOptionWindow::wxOptionWindow(wxWindow* parent, const wxString &key)
: wxWindow(parent, -1)
, mKey(key)
{
	wxASSERT(parent);

	SetBackgroundColour(SKIN.Element(shBackground));

	Show(false);
}

bool wxOptionWindow::Show(bool show)
{
	bool ret = wxWindow::Show(show);

	// Mark that we were the last option window open.
	if(true == show)
	{
		CONFIG.write<wxString>(swConfigSectionGeneral, swLastOptionOpen, mKey);
	}

	return ret;
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//


