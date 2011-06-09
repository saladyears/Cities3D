/*
 *  Cities3D - Copyright (C) 2001-2009 Jason Fugate (saladyears@gmail.com)
 * 
 *  This program is free software; you can redistribute it and/or modify it 
 *  under the terms of the GNU General Public License as published by the Free 
 *  Software Foundation; either version 2 of the License, or (at your option) 
 *  any later version.
 * 
 *  This program is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; ithout even the implied warranty of MERCHANTABILITY 
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
 *  for more details.
 */
#include "stdwx.h"
#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//
#include "BaseDialog.h"
#include "ConfigDatabase.h"
#include "ISizer.h"
#include "PopupManager.h"
#include "SkinManager.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef __WIN32__
#ifdef max
#undef max
#endif
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//
wxBaseDialog::wxBaseDialog(wxWindow *parent, const ISizerPtr &sizer, 
	const wxString &label, const wxString &config, long style)
: wxDialog(parent, -1, label, wxDefaultPosition, wxDefaultSize, style | 
		   wxWANTS_CHARS)
, mConfig(config)
{
	// Create the sizer.
	wxBoxSizer* top = new wxBoxSizer(wxHORIZONTAL);

	if(sizer)
	{
		sizer->CreateLayout(top);
	}
	
	SetSizer(top);
	top->SetSizeHints(this);

	//read size from ConfigDatabase
	wxInt32 width, height;
	wxString str = mConfig + swForwardSlash;
	width = CONFIG.read(swConfigSectionUI, str + swWidth, -1);
	height = CONFIG.read(swConfigSectionUI, str + swHeight, -1);

	wxSize bestSize = GetSizer()->GetMinSize();

	// Adjust to fit on the screen if needed.
	wxSize displaySize = wxGetDisplaySize();

	wxSize thisSize = wxSize(
		std::min(displaySize.x, std::max(width, bestSize.GetWidth())), 
		std::min(displaySize.y, std::max(height, bestSize.GetHeight())));

	SetSize(-1, -1, thisSize.x, thisSize.y);		

	// Read position from ConfigDatabase
	wxInt32 x, y;
	x = CONFIG.read(swConfigSectionUI, str + swX, -1);
	y = CONFIG.read(swConfigSectionUI, str + swY, -1);

	if( (-1 == x) &&
		(-1 == y))
	{
		CentreOnParent();
	}
	else
	{
		if(0 > x)
		{
			x = 0;
		}
		if(0 > y)
		{
			y = 0;
		}

		if(displaySize.x < x + thisSize.x)
		{
			x = displaySize.x - thisSize.x;
		}
		if(displaySize.y < y + thisSize.y)
		{
			y = displaySize.y - thisSize.y;
		}

		SetPosition(wxPoint(x, y));
	}

	// Suppress all popups while dialogs are open.
	PopupManager::get().AddWindow(this, 2);

	OnSkin(0);
}

wxBaseDialog::~wxBaseDialog()
{
	PopupManager::get().RemoveWindow(this);

	wxString str = mConfig + swForwardSlash;
	wxRect rect = GetRect();

	//save the dialog size
	CONFIG.write(swConfigSectionUI, str + swWidth, rect.width);
	CONFIG.write(swConfigSectionUI, str + swHeight, rect.height);

	// Save the dialog position.
	CONFIG.write(swConfigSectionUI, str + swX, rect.x);
	CONFIG.write(swConfigSectionUI, str + swY, rect.y);
}

void wxBaseDialog::OnSkin(wxInt32)
{
	SetBackgroundColour(SKIN.Element(shBackground));
}

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxBaseDialog, wxDialog)
	EVT_SIZE(wxBaseDialog::OnSize)
END_EVENT_TABLE()

void wxBaseDialog::OnSize(wxSizeEvent &event)
{
	Size(event);

	event.Skip();
}
