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
#include "MasterMerchantPlayerDialog.h"
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
	wxImage CreateImageMasterMerchantPlayer(ColorType color, wxInt32 total)
	{
		wxImage icon = ColorInfoData::UIImage(color, wxSize(32, 32));

		// Get it once and then reget at half size.
		wxImage card = ImageEngine::get().GetImage(shResources, shBack);

		wxInt32 w = 40 + card.GetWidth();
		wxInt32 h = std::max(card.GetHeight() + 14, 32);

		wxBitmap bitmap(w, h);
		wxBitmap iconBitmap(icon);
		wxBitmap cardBitmap(card);

		wxFont font = SKIN.Font(shPlayerSelectionSubFont);
		wxColour trans = SKIN.Element(shIconTransparent);

		wxMemoryDC dc;
		dc.SelectObject(bitmap);
		dc.SetBrush(trans);
		dc.SetPen(trans);
		dc.DrawRectangle(0, 0, w, h);
		dc.SetBackgroundMode(wxTRANSPARENT);
		dc.DrawBitmap(iconBitmap, 0, (h - 32) / 2, true);
		dc.DrawBitmap(cardBitmap, 40, (h - (card.GetHeight() + 14)) / 2, true);

		Utility::calculateFontSize(dc, font, 12);
		wxString str = wxString::Format(swIntFormat.c_str(), total);
		
		wxInt32 x, y;
		dc.GetTextExtent(str, &x, &y);
		wxInt32 startX = 40 + ((card.GetWidth() - x) / 2);
		wxInt32 startY = card.GetHeight() + 2;

		dc.SetTextForeground(*wxBLACK);
		dc.DrawText(str, startX + 1, startY + 1);

		dc.SetTextForeground(*wxWHITE);
		dc.DrawText(str, startX, startY);

		dc.SelectObject(wxNullBitmap);

		return bitmap.ConvertToImage();
	}

	void PopulateListMasterMerchantPlayer(wxBaseListCtrl* list, 
		boost::weak_ptr<Game> thisGame)
	{
		const GamePtr& game = thisGame.lock();
		wxASSERT(game);

		// Only add players that have more points than the current player.
		wxInt32 current = game->read<wxInt32>(shCurrentPlayer);
		wxInt32 points = game->playerGame(current).read<wxInt32>(shPoints);
		wxInt32 players = game->numPlayers();

		for(wxInt32 i = 0; i < players; ++i)
		{
			wxInt32 playerPoints = game->playerGame(i).read<wxInt32>(shPoints);
			if( (i != current) &&
				(points < playerPoints))
			{
				const Data::IntHash& resources = 
					game->playerGame(i).read<Data::IntHash>(shResources);
				wxInt32 total = 0;
				Data::IntHash::const_iterator it, itEnd = resources.end();
				for(it = resources.begin(); it != itEnd; ++it)
				{
					total += it->second;
				}

				if(0 < total)
				{
					ColorType color = game->playerGame(i).color();
					wxImage image = CreateImageMasterMerchantPlayer(color, total);

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
wxMasterMerchantPlayerDialog::wxMasterMerchantPlayerDialog(wxWindow *parent, 
	boost::weak_ptr<Game> game)
: wxPlayerSelectionDialog(parent, game, shRuleSelectMasterMerchantPlayer, 
	stSelectAPlayerToRobUI, boost::bind(&PopulateListMasterMerchantPlayer, _1, _2))
{
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
