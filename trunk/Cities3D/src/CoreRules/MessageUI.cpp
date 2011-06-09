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
#include "MessageUI.h"
#include "Game.h"
#include "DataObject.h"
#include "Controller.h"
#include "ColorInfoData.h"
#include "BaseStaticText.h"
#include "SkinManager.h"
#include "ConfigDatabase.h"
#include "GLExport.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	const wxSize sMinSizeMessage = wxSize(165, 40);

	wxBaseStaticText* spText = NULL;
}

//---------------------------- PUBLIC           -----------------------------//
wxMessageUI::wxMessageUI(wxWindow *parent) 
: wxBaseUI<wxPanel>(parent, sMinSizeMessage)
{
	Show(true);

	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	
	SetBackgroundColour(SKIN.Element(shBackground));

	// Create the text object.
	spText = new wxBaseStaticText(this, 0, swEmpty, shDefaultText, shGameUIFont, sMinSizeMessage, 
		wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	spText->Show();

	sizer->Add(spText, 1, wxEXPAND | wxALL, 4);

	SetSizer(sizer);
	sizer->SetSizeHints(this);

	Controller::get().AddReceiver(shEventMessageUI, 
		&wxMessageUI::OnUpdate, this);
}

wxMessageUI::~wxMessageUI()
{
	Controller::get().RemoveReceiver(shEventMessageUI, 
		&wxMessageUI::OnUpdate, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxMessageUI::OnUpdate(const DataObject &object)
{
	wxASSERT(2 == object.numItems());
	
	//read the data out of the UI
	wxString message = object.read<wxString>();
	const GamePtr &game = object.read<GamePtr>(1);

	spText->Freeze();
	spText->SetLabel(message);

	//set the label color to be the current player, unless the game has ended,
	//in which case it should be white
	if(shComplete == game->read<HashString>(shState))
	{
		wxColour colour = SKIN.Element(shGameUIText);
		spText->SetForegroundColour(colour);
	}
	else
	{
		ColorType type;
		type = game->playerGame(game->read<wxInt32>(
			shCurrentPlayer)).color();
		spText->SetForegroundColour(ColorInfoData::UIColor(type));
	}

	spText->Thaw();
	spText->Refresh();
}


