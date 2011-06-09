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
#include "RollsUI.h"
#include "ISizer.h"
#include "Controller.h"
#include "DataObject.h"
#include "ImageEngine.h"
#include "ImageCenteredBitmap.h"
#include "PlayerCenteredBitmap.h"
#include "ImageStrip.h"
#include "Game.h"
#include "ColorInfoData.h"
#include "SkinManager.h"
#include "IRuleEngine.h"
#include "CommonUI.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	class wxRollImageStrip : public wxImageStrip
	{
	public:
		wxRollImageStrip(wxWindow* parent, wxWindowID id, size_t minTextHeight)
			: wxImageStrip(parent, id, 6, minTextHeight)
		{
			SetFlags(FlagAlternateDraw);
			SetPixelSpacing(2);
		}
	};

	wxImageCenteredBitmap *spDie1 = NULL;
	wxImageCenteredBitmap *spDie2 = NULL;
	wxImageCenteredBitmap *spDie3 = NULL;

	wxSize sDefaultPlayerSize = wxSize(20, 20);
	wxSize sDefaultDieSize = wxSize(20, 20);
	wxSize sMinSize = wxSize(165, 150);

	std::vector<wxPlayerCenteredBitmap*> sBitmaps;

	typedef std::vector<wxRollImageStrip*> ImageStripArray;
	ImageStripArray sImageStrips;
}

//---------------------------- PUBLIC           -----------------------------//
wxRollsUI::wxRollsUI(wxWindow *parent, const GamePtr &game, bool threeDice)
: wxBaseUI<wxPanel>(parent, sMinSize)
, mThreeDice(threeDice)
{
	wxASSERT(game);

	SetBackgroundColour(SKIN.Element(shBackground));
	SetMinSize(sMinSize);

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
	sImageStrips.resize(players);

	// Add the player colors.
	for(wxInt32 i = 0; i < players; ++i)
	{
		sBitmaps[i] = new wxPlayerCenteredBitmap(this,
			sDefaultPlayerSize, sDefaultPlayerSize, 
			game->playerGame(i).color());
		color->Add(sBitmaps[i], 20, wxEXPAND);
		color->AddStretchSpacer(spacerY);
	}

	wxSizer* innerHorz = new wxBoxSizer(wxHORIZONTAL);

	wxInt32 xStretch = threeDice ? 10 : 25;

	// Add the dice bitmap / text displays.
	innerHorz->AddStretchSpacer(xStretch);
	spDie1 = new wxImageCenteredBitmap(this, wxSize(30, 30), wxSize(28, 28), 
		shDice, sh1);
	spDie1->SetMinSize(wxSize(30, 30));
	innerHorz->Add(spDie1, 30, wxEXPAND | wxSHAPED);

	// Second die might be red in a three dice game.
	spDie2 = new wxImageCenteredBitmap(this, wxSize(30, 30), wxSize(28, 28), 
		threeDice ? shDiceRed : shDice, threeDice ? shRed1 : sh1);
	spDie2->SetMinSize(wxSize(30, 30));
	innerHorz->Add(spDie2, 30, wxEXPAND | wxSHAPED);

	if(true == threeDice)
	{
		spDie3 = new wxImageCenteredBitmap(this, wxSize(30, 30), wxSize(28, 28), 
			shDiceCK, shBarbarians);
		spDie3->SetMinSize(wxSize(30, 30));
		innerHorz->Add(spDie3, 30, wxEXPAND | wxSHAPED);
	}
	innerHorz->AddStretchSpacer(xStretch);

	display->Add(innerHorz, 30, wxEXPAND);
	display->AddStretchSpacer(spacerY);
	
	wxSizeEvent event;

	// Finally, add each of the roll image columns.
	for(wxInt32 i = 0; i < players; ++i)
	{
		wxRollImageStrip* strip = new wxRollImageStrip(this, -1, 10);
		strip->ProcessEvent(event);
		display->Add(strip, 20, wxEXPAND | wxLEFT, 10);
		display->AddStretchSpacer(spacerY);

		sImageStrips[i] = strip;
	}

	horz->Add(color, 40, wxEXPAND | wxSHAPED);
	horz->Add(display, 110, wxEXPAND | wxSHAPED);

	sizer->Add(horz, 1, wxEXPAND | wxSHAPED);

	SetSizer(sizer);

	Controller::get().AddReceiver(shEventDiceRoll, 
		&wxRollsUI::OnDiceRoll, this);
	Controller::get().AddReceiver(shEventThirdDie, 
		&wxRollsUI::OnThirdDie, this);
	Controller::get().AddReceiver(shEventPlayerResources,
		&wxRollsUI::OnPlayerResources, this);
	Controller::get().AddReceiver(shEventPlayerReorder,
		&wxRollsUI::OnPlayerReorder, this);

	Controller::get().AddReceiver(shEventTabRolls, &wxRollsUI::OnTabRolls, 
		this);
}

wxRollsUI::~wxRollsUI()
{
	Controller::get().RemoveReceiver(shEventDiceRoll, 
		&wxRollsUI::OnDiceRoll, this);
	Controller::get().RemoveReceiver(shEventThirdDie, 
		&wxRollsUI::OnThirdDie, this);
	Controller::get().RemoveReceiver(shEventPlayerResources,
		&wxRollsUI::OnPlayerResources, this);
	Controller::get().RemoveReceiver(shEventPlayerReorder,
		&wxRollsUI::OnPlayerReorder, this);

	Controller::get().RemoveReceiver(shEventTabRolls, &wxRollsUI::OnTabRolls, 
		this);

	sImageStrips.clear();
	spDie3 = NULL;
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxRollsUI::OnDiceRoll(const DataObject &data)
{
	GetSizer()->Layout();

	wxInt32 die1 = data.read<wxInt32>();
	wxInt32 die2 = data.read<wxInt32>(1);

	spDie1->SetBitmapImage(HashString::Format(shIntFormat.c_str(), die1));

	if(true == mThreeDice)
	{
		spDie2->SetBitmapImage(HashString::Format(shRedIntFormat.c_str(), die2));
	}
	else
	{
		spDie2->SetBitmapImage(HashString::Format(shIntFormat.c_str(), die2));
	}

	// Clear all the image strips in prepration for receiving resources.
	std::for_each(sImageStrips.begin(), sImageStrips.end(),
		boost::bind(&wxImageStrip::Clear, _1));
}

void wxRollsUI::OnThirdDie(const size_t die)
{
	HashString image;
	switch(die)
	{
	case 1:
	case 2:
	case 3:
		image = shBarbarians;
		break;
	case 4:
		image = shTrade;
		break;
	case 5:
		image = shPolitics;
		break;
	case 6:
		image = shScience;
		break;
	default:
		wxASSERT(false);
		break;
	};

	spDie3->SetBitmapImage(image);
}

void wxRollsUI::OnPlayerResources(const ResourceMapArray &resources)
{
	wxASSERT(resources.size() == sImageStrips.size());

	// Get the order that resources appear in in the game.
	DataObject input(true), output;
	RULE.Decide(shLogicResourceOrder, input, output);

	typedef std::map<wxInt32, HashString> OrderMap;
	const OrderMap& map = output.read<OrderMap>();

	// Set the display in the image strips.
	for(size_t i = 0; i < resources.size(); ++i)
	{
		wxImageStrip * const strip = sImageStrips[i];

		strip->Hide();
		strip->Clear();

		// Go through the resources in order and add them if the player got
		// them.
		OrderMap::const_iterator it, itEnd = map.end();
		for(it = map.begin(); it != itEnd; ++it)
		{
			const HashString& res = it->second;

			ResourceMap::const_iterator itMap = resources[i].find(res);
			if(resources[i].end() != itMap)
			{
				wxInt32 count = itMap->second;

				if(0 < count)
				{
					strip->Add(res, count);
				}
			}
		}

		strip->Show();
	}
}

void wxRollsUI::OnPlayerReorder(const GamePtr& game)
{
	// Go through and change the colors of each of the bitmaps.
	for(wxInt32 i = 0; i < game->numPlayers(); ++i)
	{
		ColorType color = game->playerGame(i).color();
		sBitmaps[i]->SetPlayerColor(color);
	}
}

void wxRollsUI::OnTabRolls(wxInt32)
{
	RaiseTab(this);
}
