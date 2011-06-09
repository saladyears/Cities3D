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
#include "UpgradeUI.h"
#include "Game.h"
#include "Controller.h"
#include "ImageCenteredBitmap.h"
#include "PlayerCenteredBitmap.h"
#include "SkinManager.h"
#include "CommonUI.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	const wxSize sMinSizeUpgrade(165, 150);

	wxSize sDefaultPlayerSize = wxSize(20, 20);
	wxSize sDefaultDieSize = wxSize(20, 20);

	std::vector<wxPlayerCenteredBitmap *> sBitmaps;

	void CreateColumn(wxWindow* window, const HashString& type, 
		const GamePtr& game, wxSizer* vert, wxInt32 spacerY,
		std::vector<wxImageCenteredBitmap*>& array)
	{
		vert->AddStretchSpacer(spacerY);

		wxInt32 players = game->numPlayers();

		for(wxInt32 i = 0; i < players; ++i)
		{
			wxInt32 level = game->playerGame(i).read<wxInt32>(type);
			HashString str = HashString::Format(shRedInt.c_str(), level + 1);
			bool display = (0 != level);

			wxImageCenteredBitmap* bitmap = new wxImageCenteredBitmap(window, 
				sDefaultDieSize, sDefaultDieSize, shDiceRed, str, display);
			vert->Add(bitmap, 20, wxEXPAND);
			vert->AddStretchSpacer(spacerY);

			array[i] = bitmap;
		}
	}

	void UpdateColumn(const HashString& type, wxInt32 player, 
		const GamePtr& game, std::vector<wxImageCenteredBitmap*>& array)
	{
		wxASSERT(static_cast<size_t>(player) < array.size());

		wxImageCenteredBitmap* bitmap = array[player];

		// Get their current level, and see if it has changed.
		wxInt32 level = game->playerGame(player).read<wxInt32>(type);
		HashString str = HashString::Format(shRedInt.c_str(), level + 1);

		const HashString& old = bitmap->GetBitmapImage();

		if(old != str)
		{
			bitmap->SetBitmapImage(str);
			bitmap->Display(0 != level);
		}
	}
}

//---------------------------- PUBLIC           -----------------------------//
wxUpgradeUI::wxUpgradeUI(wxWindow* parent, const GamePtr &game)
: wxBaseUI<wxPanel>(parent, sMinSizeUpgrade)
{
	SetMinSize(sMinSizeUpgrade);
	SetBackgroundColour(SKIN.Element(shBackground));

	Controller::get().AddReceiver(shEventCityUpgrade, 
		&wxUpgradeUI::OnCityUpgrade, this);
	Controller::get().AddReceiver(shEventPlayerReorder, 
		&wxUpgradeUI::OnPlayerReorder, this);
	Controller::get().AddReceiver(shEventTabUpgrades,
		&wxUpgradeUI::OnTabUpgrades, this);

	wxInt32 players = game->numPlayers();

	mTrade.resize(players, 0);
	mPolitics.resize(players, 0);
	mScience.resize(players, 0);

	// Create all of the bitmap images to hold everything.
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	// Create the four vertical column sizers for player colors and the three
	// types of upgrades.
	wxBoxSizer* color = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* vert1 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* vert2 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* vert3 = new wxBoxSizer(wxVERTICAL);

	wxInt32 spacerY = 140;
	spacerY -= (players * 20);
	spacerY /= (players + 1);

	float ratio = static_cast<float>(players) / 6.0f;

	spacerY *= static_cast<wxInt32>(ratio);
	wxInt32 spacerX = static_cast<wxInt32>(20 * ratio);

	// Color column.
	color->AddStretchSpacer(30);
	color->AddStretchSpacer(spacerY);

	sBitmaps.resize(players);

	for(wxInt32 i = 0; i < players; ++i)
	{
		sBitmaps[i] = new wxPlayerCenteredBitmap(this,
			sDefaultPlayerSize, sDefaultPlayerSize, 
			game->playerGame(i).color());
		color->Add(sBitmaps[i], 20, wxEXPAND);
		color->AddStretchSpacer(spacerY);
	}
	
	// The three card images.
	wxImageCenteredBitmap* trade = new wxImageCenteredBitmap(this, 
		wxSize(20, 30),	wxSize(16, 22), shCards, shTradeCard);
	wxImageCenteredBitmap* politics = new wxImageCenteredBitmap(this, 
		wxSize(20, 30),	wxSize(16, 22), shCards, shPoliticsCard);
	wxImageCenteredBitmap* science = new wxImageCenteredBitmap(this, 
		wxSize(20, 30),	wxSize(16, 22), shCards, shScienceCard);

	// Columns.
	vert1->Add(trade, 30, wxEXPAND);
	vert2->Add(politics, 30, wxEXPAND);
	vert3->Add(science, 30, wxEXPAND);
	
	CreateColumn(this, shTradeLevel, game, vert1, spacerY, mTrade);
	CreateColumn(this, shPoliticsLevel, game, vert2, spacerY, mPolitics);
	CreateColumn(this, shScienceLevel, game, vert3, spacerY, mScience);

	sizer->Add(color, 25, wxEXPAND);
	sizer->AddStretchSpacer(spacerX);
	sizer->Add(vert1, 20, wxEXPAND);
	sizer->AddStretchSpacer(spacerX);
	sizer->Add(vert2, 20, wxEXPAND);
	sizer->AddStretchSpacer(spacerX);
	sizer->Add(vert3, 20, wxEXPAND);
	sizer->AddStretchSpacer(spacerX);
	
	SetSizer(sizer);
}

wxUpgradeUI::~wxUpgradeUI()
{
	Controller::get().RemoveReceiver(shEventCityUpgrade, 
		&wxUpgradeUI::OnCityUpgrade, this);
	Controller::get().RemoveReceiver(shEventPlayerReorder, 
		&wxUpgradeUI::OnPlayerReorder, this);
	Controller::get().RemoveReceiver(shEventTabUpgrades,
		&wxUpgradeUI::OnTabUpgrades, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxUpgradeUI::OnCityUpgrade(const GamePtr& game)
{
	// Set the new bitmaps.
	wxInt32 currentPlayer = game->read<wxInt32>(shCurrentPlayer);

	UpdateColumn(shTradeLevel, currentPlayer, game, mTrade);
	UpdateColumn(shPoliticsLevel, currentPlayer, game, mPolitics);
	UpdateColumn(shScienceLevel, currentPlayer, game, mScience);

}

void wxUpgradeUI::OnPlayerReorder(const GamePtr& game)
{
	// Go through and change the colors of each of the bitmaps.
	for(size_t i = 0; i < sBitmaps.size(); ++i)
	{
		ColorType color = game->playerGame(i).color();
		sBitmaps[i]->SetPlayerColor(color);
	}
}

void wxUpgradeUI::OnTabUpgrades(wxInt32)
{
	RaiseTab(this);
}
