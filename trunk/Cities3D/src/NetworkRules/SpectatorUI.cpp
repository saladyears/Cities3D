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
#include "SpectatorUI.h"
#include "SpectatorListCtrl.h"
#include "CommonUI.h"
#include "Controller.h"

//---------------------------- USER INCLUDES    -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
wxSpectatorUI::wxSpectatorUI(wxWindow *parent)
: wxBaseUI<wxPanel>(parent, wxDefaultSize)
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxSpectatorListCtrl* spectators = new wxSpectatorListCtrl(this ,wxID_ANY);
	sizer->Add(spectators, 1, wxEXPAND);

	SetSizer(sizer);
	sizer->SetSizeHints(this);

	Controller::get().AddReceiver(shEventTabSpectators,
		&wxSpectatorUI::OnTabSpectators, this);
}

wxSpectatorUI::~wxSpectatorUI()
{
	Controller::get().RemoveReceiver(shEventTabSpectators, 
		&wxSpectatorUI::OnTabSpectators, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxSpectatorUI::OnTabSpectators(wxInt32)
{
	RaiseTab(this);
}
