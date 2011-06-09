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
#include "OpenGamePane.h"
#include "GameListCtrl.h"
#include "BaseButton.h"
#include "GameEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	enum
	{
		ID_GAME_LIST		= 0,
	};

	//controls
	wxGameListCtrl *spGameList;
	wxBaseButton *spOKOpen;

	void CreatePaneOpenGame(wxWindow* window)
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		wxBoxSizer *vert1 = new wxBoxSizer(wxVERTICAL);
		spGameList = new wxGameListCtrl(window, ID_GAME_LIST, 
			wxDefaultPosition, wxSize(350, 300));
		vert1->Add(spGameList, 1, wxEXPAND | wxALL, 3);

		sizer->Add(vert1, 1, wxEXPAND);

		// Button.
		spOKOpen = new wxBaseButton(window, wxID_OK, stOK);
		spOKOpen->Enable(false);
		
		sizer->Add(spOKOpen, 0, wxALL, 3);
		
		window->SetSizer(sizer);
		sizer->SetSizeHints(window);
	}
}

//---------------------------- PUBLIC           -----------------------------//
wxOpenGamePane::wxOpenGamePane(wxWindow *parent)
: wxPanel(parent, wxID_ANY, wxPoint(parent->GetClientSize().x,
	parent->GetClientSize().y))
, PaneMixin(this)
{
	CreatePaneOpenGame(this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxOpenGamePane, wxPanel)
	EVT_LIST_ITEM_ACTIVATED(ID_GAME_LIST, wxOpenGamePane::OnGameDoubleClick)
	EVT_LIST_ITEM_SELECTED(ID_GAME_LIST, wxOpenGamePane::OnGameSelected)
	EVT_LIST_ITEM_DESELECTED(ID_GAME_LIST, wxOpenGamePane::OnGameSelected)
	EVT_BUTTON(wxID_OK, wxOpenGamePane::OnOk)
END_EVENT_TABLE()

void wxOpenGamePane::OnOk(wxCommandEvent&)
{
	wxInt32 index = spGameList->GetSelectedItem();
	
	//sanity
	if(-1 != index)
	{
		GameEngine::get().LoadGame(index);
	}
}

void wxOpenGamePane::OnGameDoubleClick(wxListEvent &)
{
	wxCommandEvent event;

	OnOk(event);
}

void wxOpenGamePane::OnGameSelected(wxListEvent &)
{
	wxInt32 index = spGameList->GetSelectedItem();

	spOKOpen->Enable(-1 != index);
}
