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
#include "CitiesOnlineTaskbarIcon.h"
#include "CitiesOnlineApp.h"
#include "BaseMessageBox.h"

#ifndef __WIN32__
#include "CitiesOnline.xpm"
#endif

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	enum
	{
		ID_EXIT			= 0,
		ID_URL			,
	};
}

//---------------------------- PUBLIC           -----------------------------//
wxCitiesOnlineTaskbarIcon::wxCitiesOnlineTaskbarIcon()
{
	// Set the icon for the task bar.
	wxIcon icon(wxICON(a_citiesonline));

	SetIcon(icon);
}

wxMenu* wxCitiesOnlineTaskbarIcon::CreatePopupMenu()
{
    static const wxString stExit = _("Exit");

	wxMenu* menu = new wxMenu();

	menu->Append(ID_EXIT, stExit, swCitiesOnline);

	return menu;
}

wxInt32 wxCitiesOnlineTaskbarIcon::GetUrlMenuId() const
{
	return ID_URL;
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxCitiesOnlineTaskbarIcon, wxTaskBarIcon)
	EVT_MENU(ID_EXIT, wxCitiesOnlineTaskbarIcon::OnMenuExit)
	EVT_MENU(ID_URL, wxCitiesOnlineTaskbarIcon::OnMenuUrl)
END_EVENT_TABLE() 

void wxCitiesOnlineTaskbarIcon::OnMenuExit(wxCommandEvent&)
{
	bool close = true;

	// If we are currently in a game, warn them of the dire consequences of
	// closing Cities Online.
	if(true == wxTheCitiesOnlineApp->InGame())
	{
        static const wxString stQuestion = _("Question");
        static const wxString stYouAreCurrentlyInACitiesOnlineGame = _("You are currently in a Cities Online game!  If you close Cities Online, your game will also close, and it may go on your record!  Are you sure you want to continue?");
    
		wxBaseMessageBox dialog(NULL, stQuestion, 
			stYouAreCurrentlyInACitiesOnlineGame, MessageBoxYesNo);

		if(wxNO == dialog.ShowModal())
		{
			close = false;
		}
	}

	if(true == close)
	{
		wxTheCitiesOnlineApp->ExitMainLoop();
	}
}

void wxCitiesOnlineTaskbarIcon::OnMenuUrl(wxCommandEvent&)
{
	wxTheCitiesOnlineApp->ProcessUrl();
}
