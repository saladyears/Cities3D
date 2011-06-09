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
#include "BaseFrame.h"
#include "SkinManager.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//
wxBaseFrame::wxBaseFrame(wxWindow* parent, const wxString& title, 
	const wxPoint& pos, const wxSize& size)
: wxFrame(parent, -1, title, pos, size)
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	SetSizer(sizer);
	sizer->SetSizeHints(this);

	OnSkin(0);

	Controller::get().AddReceiver(shEventSkin, &wxBaseFrame::OnSkin, this);
	Controller::get().AddReceiver(shEventToggleFullscreen, 
        &wxBaseFrame::OnToggleFullscreen, this);
}

wxBaseFrame::~wxBaseFrame()
{
	Controller::get().RemoveReceiver(shEventSkin, &wxBaseFrame::OnSkin, this);
	Controller::get().RemoveReceiver(shEventToggleFullscreen, 
        &wxBaseFrame::OnToggleFullscreen, this);
}

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxBaseFrame, wxFrame)
	EVT_SIZE(wxBaseFrame::OnSize)
END_EVENT_TABLE()

void wxBaseFrame::OnSize(wxSizeEvent &event)
{
	Size(event);

	event.Skip();
}

void wxBaseFrame::OnSkin(wxInt32)
{
	SetBackgroundColour(SKIN.Element(shBackground));
}

void wxBaseFrame::OnToggleFullscreen(wxInt32)
{
	ShowFullScreen(!IsFullScreen());
	Refresh();
}
