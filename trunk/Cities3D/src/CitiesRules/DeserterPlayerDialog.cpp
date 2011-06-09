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
#include "DeserterPlayerDialog.h"
#include "Game.h"
#include "BaseListCtrl.h"
#include "ColorInfoData.h"
#include "ImageEngine.h"
#include "SkinManager.h"
#include "Utility.h"
#include "KnightObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	void CountKnights(const GamePtr& game, wxInt32 player, wxInt32& level1,
		wxInt32& level2, wxInt32& level3)
	{
		level1 = level2 = level3 = 0;

		const PlayerGame::CornerObjectArray& knights =
			game->playerGame(player).getCornerObjects(shKnights);
		PlayerGame::CornerObjectArray::const_iterator it,
			itEnd = knights.end();
		for(it = knights.begin(); it != itEnd; ++it)
		{
			KnightObject* knight = static_cast<KnightObject*>(it->get());
			switch(knight->strength())
			{
			case 1:
				++level1;
				break;
			case 2:
				++level2;
				break;
			case 3:
				++level3;
				break;
			default:
				wxASSERT(false);
				break;
			}
		}
	}

	wxImage CreateImageDeserterPlayer(ColorType color, const GamePtr& game, 
		wxInt32 player)
	{
		wxSize size = wxSize(32, 32);
		wxImage icon = ColorInfoData::UIImage(color, size );

		// Get the knight images
		wxImage knight1 = ImageEngine::get().Colorize(shKnight1128, shKnight128Mask, 
			color, size, true);
		wxImage knight2 = ImageEngine::get().Colorize(shKnight2128, shKnight128Mask, 
			color, size, true);
		wxImage knight3 = ImageEngine::get().Colorize(shKnight3128, shKnight128Mask, 
			color, size, true);

		wxInt32 wid = knight1.GetWidth();
		wxInt32 hei = knight1.GetHeight();
		wxInt32 w = 42 + (wid  * 3);
		wxInt32 h = std::max(hei + 14, 32);

		wxBitmap bitmap(w, h);
		wxBitmap iconBitmap(icon);
		wxBitmap knight1Bitmap(knight1);
		wxBitmap knight2Bitmap(knight2);
		wxBitmap knight3Bitmap(knight3);

		wxInt32 level1, level2, level3;
		CountKnights(game, player, level1, level2, level3);
		
		wxFont font = SKIN.Font(shPlayerSelectionSubFont);
		wxColour trans = SKIN.Element(shIconTransparent);

		wxMemoryDC dc;
		dc.SelectObject(bitmap);
		dc.SetBrush(trans);
		dc.SetPen(trans);
		dc.DrawRectangle(0, 0, w, h);
		dc.SetBackgroundMode(wxTRANSPARENT);
		dc.DrawBitmap(iconBitmap, 0, (h - 32) / 2, true);

		wxInt32 x = 40;
		wxInt32 y = hei;

		Utility::calculateFontSize(dc, font, 12);

		if(0 < level1)
		{
			dc.DrawBitmap(knight1Bitmap, x, (h - (y + 14)) / 2, true);
			wxString str = wxString::Format(swIntFormat.c_str(), level1);

			wxInt32 x1, y1;
			dc.GetTextExtent(str, &x1, &y1);
			wxInt32 startX = x + ((wid - x1) / 2);
			wxInt32 startY = hei + 2;

			dc.SetTextForeground(*wxBLACK);
			dc.DrawText(str, startX + 1, startY + 1);

			dc.SetTextForeground(*wxWHITE);
			dc.DrawText(str, startX, startY);
		
		}
		x += wid + 1;

		if(0 < level2)
		{
			dc.DrawBitmap(knight2Bitmap, x, (h - (y + 14)) / 2, true);
			wxString str = wxString::Format(swIntFormat.c_str(), level2);

			wxInt32 x1, y1;
			dc.GetTextExtent(str, &x1, &y1);
			wxInt32 startX = x + ((wid - x1) / 2);
			wxInt32 startY = hei + 2;

			dc.SetTextForeground(*wxBLACK);
			dc.DrawText(str, startX + 1, startY + 1);

			dc.SetTextForeground(*wxWHITE);
			dc.DrawText(str, startX, startY);
		}
		x += wid + 1;

		if(0 < level3)
		{
			dc.DrawBitmap(knight3Bitmap, x, (h - (y + 14)) / 2, true);
			wxString str = wxString::Format(swIntFormat.c_str(), level3);

			wxInt32 x1, y1;
			dc.GetTextExtent(str, &x1, &y1);
			wxInt32 startX = x + ((wid - x1) / 2);
			wxInt32 startY = hei + 2;

			dc.SetTextForeground(*wxBLACK);
			dc.DrawText(str, startX + 1, startY + 1);

			dc.SetTextForeground(*wxWHITE);
			dc.DrawText(str, startX, startY);
		}

		dc.SelectObject(wxNullBitmap);

		return bitmap.ConvertToImage();
	}

	void PopulateListDeserterPlayer(wxBaseListCtrl* list, 
		boost::weak_ptr<Game> thisGame)
	{
		const GamePtr& game = thisGame.lock();
		wxASSERT(game);

		// Only add players that have progress cards.
		wxInt32 current = game->read<wxInt32>(shCurrentPlayer);
		wxInt32 players = game->numPlayers();

		for(wxInt32 i = 0; i < players; ++i)
		{
			if(i != current)
			{
				wxInt32 level1, level2, level3;
				CountKnights(game, i, level1, level2, level3);

				if(0 < (level1 | level2 | level3))
				{
					ColorType color = game->playerGame(i).color();
					wxImage image = CreateImageDeserterPlayer(color, game, i);
					wxInt32 playerPoints = 
						game->playerGame(i).read<wxInt32>(shPoints);

					const wxString& name = game->playerGame(i).player().Name();
					wxString str = wxString::Format(swStringParenInt.c_str(), 
                        name.c_str(), playerPoints);
					size_t index = list->AppendItem(str, image);
					list->SetItemData(index, i);
				}
			}
		}
	}
}

//---------------------------- PUBLIC           -----------------------------//
wxDeserterPlayerDialog::wxDeserterPlayerDialog(wxWindow *parent, 
	boost::weak_ptr<Game> game)
: wxPlayerSelectionDialog(parent, game, shRuleSelectDeserterPlayer, 
	stSelectAPlayerForDesertionUI, boost::bind(&PopulateListDeserterPlayer, _1, _2))
{
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
