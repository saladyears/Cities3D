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
#include "SpyPlayerDialog.h"
#include "Game.h"
#include "BaseListCtrl.h"
#include "ColorInfoData.h"
#include "ImageEngine.h"
#include "SkinManager.h"
#include "Utility.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	wxInt32 CountHash(const GamePtr& game, const HashString& type, 
		wxInt32 player)
	{
		wxASSERT(game);

		wxInt32 total = 0;

		const Data::IntHash& cards = 
			game->playerGame(player).read<Data::IntHash>(type);
		Data::IntHash::const_iterator it, itEnd = cards.end();
		for(it = cards.begin(); it != itEnd; ++it)
		{
			total += it->second;
		}

		return total;
	}

	wxImage CreateImageSpyPlayer(ColorType color, const GamePtr& game, 
		wxInt32 player)
	{
		wxImage icon = ColorInfoData::UIImage(color, wxSize(32, 32));

		// Get the card backs
		wxImage trade = ImageEngine::get().GetImage(shCards, shTradeCard);
		wxImage politics = ImageEngine::get().GetImage(shCards, shPoliticsCard);
		wxImage science = ImageEngine::get().GetImage(shCards, shScienceCard);

		wxInt32 wid = trade.GetWidth();
		wxInt32 hei = trade.GetHeight();
		wxInt32 w = 42 + (wid  * 3);
		wxInt32 h = std::max(hei + 14, 32);

		wxBitmap bitmap(w, h);
		wxBitmap iconBitmap(icon);
		wxBitmap tradeBitmap(trade);
		wxBitmap politicsBitmap(politics);
		wxBitmap scienceBitmap(science);

		wxInt32 tradeCount = CountHash(game, shPlayableTradeCards, player);
		wxInt32 politicsCount = CountHash(game, shPlayablePoliticsCards, player);
		wxInt32 scienceCount = CountHash(game, shPlayableScienceCards, player);

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

		if(0 < tradeCount)
		{
			dc.DrawBitmap(tradeBitmap, x, (h - (y + 14)) / 2, true);
			wxString str = wxString::Format(swIntFormat.c_str(), tradeCount);

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

		if(0 < politicsCount)
		{
			dc.DrawBitmap(politicsBitmap, x, (h - (y + 14)) / 2, true);
			wxString str = wxString::Format(swIntFormat.c_str(), politicsCount);

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

		if(0 < scienceCount)
		{
			dc.DrawBitmap(scienceBitmap, x, (h - (y + 14)) / 2, true);
			wxString str = wxString::Format(swIntFormat.c_str(), scienceCount);

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

	wxInt32 CountCards(const GamePtr& game, wxInt32 player)
	{
		wxInt32 total = 0;

		total += CountHash(game, shPlayableTradeCards, player);
		total += CountHash(game, shPlayablePoliticsCards, player);
		total += CountHash(game, shPlayableScienceCards, player);

		return total;
	}

	void PopulateListSpyPlayer(wxBaseListCtrl* list, 
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
				wxInt32 total = CountCards(game, i);
				if(0 <total)
				{
					ColorType color = game->playerGame(i).color();
					wxImage image = CreateImageSpyPlayer(color, game, i);
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
wxSpyPlayerDialog::wxSpyPlayerDialog(wxWindow *parent, 
	boost::weak_ptr<Game> game)
: wxPlayerSelectionDialog(parent, game, shRuleSelectSpyPlayer, 
	stSelectAPlayerToSpyOnUI, boost::bind(&PopulateListSpyPlayer, _1, _2))
{
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
