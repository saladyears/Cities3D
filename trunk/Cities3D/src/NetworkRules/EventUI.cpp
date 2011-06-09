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
#include "EventUI.h"
#include "EventWindow.h"
#include "CommonUI.h"
#include "Controller.h"

//---------------------------- USER INCLUDES    -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
wxEventUI::wxEventUI(wxWindow *parent)
: wxBaseUI<wxPanel>(parent, wxDefaultSize)
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxEventWindow* chat = new wxEventWindow(this, wxID_ANY);
	sizer->Add(chat, 1, wxEXPAND);

	SetSizer(sizer);
	sizer->SetSizeHints(this);

	Controller::get().AddReceiver(shEventTabEvents, &wxEventUI::OnTabEvents, 
		this);
}

wxEventUI::~wxEventUI()
{
	Controller::get().RemoveReceiver(shEventTabEvents, 
		&wxEventUI::OnTabEvents, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxEventUI::OnTabEvents(wxInt32)
{
	RaiseTab(this);
}
