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
#include "EruptionUI.h"
#include "Controller.h"
#include "PlayerCenteredBitmap.h"
#include "ImageCenteredBitmap.h"
#include "BaseStaticText.h"
#include "SkinManager.h"
#include "Game.h"
#include "CommonUI.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	wxImageCenteredBitmap *spDie1 = NULL;

	wxSize sDefaultPlayerSize = wxSize(20, 20);
	wxSize sDefaultDieSize = wxSize(20, 20);
	wxSize sMinSize = wxSize(165, 150);

	std::vector<wxPlayerCenteredBitmap*> sBitmaps;
	std::vector<wxBaseStaticText*> sTexts;
}

//---------------------------- PUBLIC           -----------------------------//
wxEruptionUI::wxEruptionUI(wxWindow *parent, const GamePtr &game)
: wxBaseUI<wxPanel>(parent, sMinSize)
{
	wxASSERT(game);

	SetBackgroundColour(SKIN.Element(shBackground));

	// Use two vertical sizers to organize everything.
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* horz = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* color = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* display = new wxBoxSizer(wxVERTICAL);
	
	wxInt32 players = game->numPlayers();

	// Calculate player spacing.
	wxInt32 spacerY = 140;
	spacerY -= (players * 20);
	spacerY /= (players + 1);

	float ratio = static_cast<float>(players) / 6.0f;

	spacerY *= static_cast<wxInt32>(ratio);

	// Color column.
	color->AddStretchSpacer(30);
	color->AddStretchSpacer(spacerY);

	sBitmaps.resize(players);
	sTexts.resize(players);

	for(wxInt32 i = 0; i < players; ++i)
	{
		sBitmaps[i] = new wxPlayerCenteredBitmap(this,
			sDefaultPlayerSize, sDefaultPlayerSize, 
			game->playerGame(i).color());
		color->Add(sBitmaps[i], 20, wxEXPAND);
		color->AddStretchSpacer(spacerY);
	}

	wxSizer* innerHorz = new wxBoxSizer(wxHORIZONTAL);

	wxInt32 xStretch = 40;

	// Add the dice bitmap / text displays.
	innerHorz->AddStretchSpacer(xStretch);
	spDie1 = new wxImageCenteredBitmap(this, wxSize(30, 30), wxSize(28, 28), 
		shDice, sh1);
	spDie1->SetMinSize(wxSize(30, 30));
	innerHorz->Add(spDie1, 30, wxEXPAND | wxSHAPED);
	
	innerHorz->AddStretchSpacer(xStretch);

	display->Add(innerHorz, 30, wxEXPAND);
	display->AddStretchSpacer(spacerY);
		
	for(wxInt32 i = 0; i < players; ++i)
	{
		sTexts[i] = new wxBaseStaticText(this, -1, swEmpty, shDefaultText, 
			shDefaultFont, wxSize(110, 25));
		display->Add(sTexts[i], 20, wxEXPAND | wxLEFT, 10);
		display->AddStretchSpacer(spacerY);
	}

	horz->Add(color, 40, wxEXPAND | wxSHAPED);
	horz->Add(display, 110, wxEXPAND | wxSHAPED);

	sizer->Add(horz, 1, wxEXPAND | wxSHAPED);

	SetSizer(sizer);

	Controller::get().AddReceiver(shEventVolcanoRoll, 
		&wxEruptionUI::OnVolcanoRoll, this);
	Controller::get().AddReceiver(shEventVolcanoResult, 
		&wxEruptionUI::OnVolcanoResult, this);
	Controller::get().AddReceiver(shEventPlayerReorder,
		&wxEruptionUI::OnPlayerReorder, this);
	Controller::get().AddReceiver(shEventTabVolcano,
		&wxEruptionUI::OnTabVolcano, this);
}

wxEruptionUI::~wxEruptionUI()
{
	Controller::get().RemoveReceiver(shEventVolcanoRoll, 
		&wxEruptionUI::OnVolcanoRoll, this);
	Controller::get().RemoveReceiver(shEventVolcanoResult, 
		&wxEruptionUI::OnVolcanoResult, this);
	Controller::get().RemoveReceiver(shEventPlayerReorder,
		&wxEruptionUI::OnPlayerReorder, this);
	Controller::get().RemoveReceiver(shEventTabVolcano,
		&wxEruptionUI::OnTabVolcano, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxEruptionUI::OnVolcanoRoll(wxInt32 roll)
{
	spDie1->SetBitmapImage(HashString::Format(shIntFormat.c_str(), roll));
}

void wxEruptionUI::OnVolcanoResult(const StringArray& results)
{
	wxASSERT(results.size() == sTexts.size());

	for(size_t i = 0; i < results.size(); ++i)
	{
		sTexts[i]->SetLabel(results[i]);
	}
}

void wxEruptionUI::OnPlayerReorder(const GamePtr& game)
{
	// Go through and change the colors of each of the bitmaps.
	for(wxInt32 i = 0; i < game->numPlayers(); ++i)
	{
		ColorType color = game->playerGame(i).color();
		sBitmaps[i]->SetPlayerColor(color);
	}
}

void wxEruptionUI::OnTabVolcano(wxInt32)
{
	RaiseTab(this);
}
