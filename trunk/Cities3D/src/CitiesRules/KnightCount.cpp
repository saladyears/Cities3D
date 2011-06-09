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
#include "KnightCount.h"
#include "ImageEngine.h"
#include "Game.h"
#include "SkinManager.h"
#include "Utility.h"
#include "KnightObject.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	wxFont sFont;
	
	wxColour sWinning;
	wxColour sLosing;
	wxColour sBackground;

	wxInt32 sCurve;
}

//---------------------------- PUBLIC           -----------------------------//
KnightCount::KnightCount(wxWindow *parent, wxInt32 index, const GamePtr& game,
	const wxPoint& pos, const wxSize& size)
: wxOwnerDrawWindow(parent, -1, pos, size)
, mPlayerIndex(index)
, mTextHeight(10)
, mState(StateNeutral)
, mPlayerColor(CR_WHITE)
, mText(wxT("0"))
{
	mPlayerColor = game->playerGame(index).color();

	Update(game);
	OnSkin();

	Controller::get().AddReceiver(shEventSkin, &KnightCount::OnSkin, this);
	Controller::get().AddReceiver(shEventImageUpdate, 
		&KnightCount::OnImageUpdate, this);
}

KnightCount::~KnightCount()
{
	Controller::get().RemoveReceiver(shEventSkin, &KnightCount::OnSkin, this);
	Controller::get().RemoveReceiver(shEventImageUpdate, 
		&KnightCount::OnImageUpdate, this);
}

void KnightCount::SetPlayerColor(ColorType color)
{
	mPlayerColor = color;
	Resize(wxSize(mBitmap.GetWidth(), mBitmap.GetHeight()), mTextHeight);
}

void KnightCount::Update(const GamePtr& game)
{
	wxASSERT(game);

	wxInt32 strength = 0;

	// Get the array of knights for this player.
	const PlayerGame::CornerObjectArray& knights = 
		game->playerGame(mPlayerIndex).getCornerObjects(shKnights);
	
	PlayerGame::CornerObjectArray::const_iterator it, 
		itEnd = knights.end();
	for(it = knights.begin(); it != knights.end(); ++it)
	{
		KnightObject* knight = static_cast<KnightObject*>(it->get());

		if(true == knight->active())
		{
			strength += knight->strength();
		}
	}

	wxString str = wxString::Format(swIntFormat.c_str(), strength);

	if(str != mText)
	{
		mText = str;

		Refresh();
	}
}

void KnightCount::SetState(State state)
{
	mState = state;

	Refresh();
}

void KnightCount::Resize(const wxSize &bitmapSize, wxInt32 textHeight)
{
	// Get the active knight bitmap for this player.
	wxImage image = IMAGE.Colorize(shKnight132a, shKnight32Mask, 
		mPlayerColor, bitmapSize);
	mBitmap = wxBitmap(image);

	mTextHeight = textHeight;

	OnSkin();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void KnightCount::Paint(wxMemoryDC &dc)
{
	// Background
	wxColour background(sBackground);
	dc.SetBrush(wxBrush(background, wxSOLID));
	dc.SetPen(wxPen(background, 1, wxSOLID));
	wxSize size = GetSize();
	dc.DrawRectangle(0, 0, size.x, size.y);

	// Draw the bitmap and the text.
	dc.DrawBitmap(mBitmap, 1, 1);

	// Draw the text.
	dc.SetFont(sFont);
	dc.SetTextForeground(wxColour(255, 255, 255));
	dc.SetTextBackground(sBackground);

	wxInt32 x, y;
	dc.GetTextExtent(mText, &x, &y);

	y = (size.y - y) / 2;

	dc.DrawText(mText, mBitmap.GetWidth() + 3, y);

	// If we are winning or losing, outline appropriately.
	if(StateNeutral != mState)
	{
		wxColour outline = (mState == StateWinning) ? sWinning : sLosing;

		dc.SetPen(wxPen(outline));
		dc.SetBrush(*wxTRANSPARENT_BRUSH);

		dc.DrawRoundedRectangle(0, 0, size.x, size.y, sCurve);
	}
}

void KnightCount::OnSkin(wxInt32)
{
	sFont = SKIN.Font(shDefaultFont);
	wxClientDC dc(this);
	Utility::calculateFontSize(dc, sFont, mTextHeight);

	sWinning = SKIN.Element(shPlayerUIPositive);
	sLosing = SKIN.Element(shPlayerUINegative);
	sBackground = SKIN.Element(shBackground);

	sCurve = SKIN.Size(shGameUICurve);

	Refresh();
}

void KnightCount::OnImageUpdate(bool)
{
	wxSize size(mBitmap.GetWidth(), mBitmap.GetHeight());
	Resize(size, mTextHeight);
}
