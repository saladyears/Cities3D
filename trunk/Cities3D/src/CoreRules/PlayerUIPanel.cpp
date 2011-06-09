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
#include <map>

//---------------------------- USER INCLUDES    -----------------------------//
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "PlayerUIPanel.h"
#include "PlayerUI.h"
#include "SkinManager.h"
#include "Game.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
wxPlayerUIPanel::wxPlayerUIPanel(wxWindow* parent, wxSettlersGLCanvas* canvas,
	const GamePtr& game)
: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0)
{
	SetBackgroundColour(SKIN.Element(shBackground));

	wxSizer* vert = new wxBoxSizer(wxVERTICAL);
	vert->AddSpacer(1);

	wxSize minSize = wxSize(0, 0);
	wxSize maxMinSize = wxSize(0, 0);

	wxInt32 players = game->numPlayers();

	typedef std::vector<wxPlayerUI*> PlayerUIArray;
	PlayerUIArray uiArray;

	// Create a UI for each player.
	for(wxInt32 i = 0; i < players; ++i)
	{
		wxPlayerUI* ui = new wxPlayerUI(i, this, game, canvas);
		uiArray.push_back(ui);
		
		wxSize size = ui->GetMinSize();
		minSize.x = std::max(minSize.x, size.x);
		minSize.y += size.y + 1;

		maxMinSize.x = std::max(maxMinSize.x, size.x);
		maxMinSize.y = std::max(maxMinSize.y, size.y);
	}

	PlayerUIArray::iterator it, itEnd = uiArray.end();
	for(it = uiArray.begin(); it != itEnd; ++it)
	{
		wxPlayerUI* ui = (*it);

		ui->SetSize(maxMinSize);
		ui->SetMinSize(maxMinSize);

		wxSizer* horz = new wxBoxSizer(wxHORIZONTAL);
		horz->Add(ui, 1, wxEXPAND);
		vert->Add(horz, players, wxEXPAND);
	}

    vert->AddStretchSpacer();

	minSize.x += 2;
	minSize.y += 1;

	SetMinSize(minSize);
	SetSizer(vert);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
