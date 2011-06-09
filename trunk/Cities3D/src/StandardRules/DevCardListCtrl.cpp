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
#include "DevCardListCtrl.h"
#include "GameEngine.h"
#include "Game.h"
#include "IRuleEngine.h"
#include "DataObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	void Populate(const GamePtr& game, wxInt32 player, 
        wxCardListCtrl::CardArray& cards)
	{
		// Now that we have the player, get their game data.
		wxASSERT(-1 != player);
		const PlayerGame& playerGame = game->playerGame(player);
		const Data::IntHash& playableCards = 
			playerGame.read<Data::IntHash>(shPlayableCards);

		// Add all playable cards.
		Data::IntHash::const_iterator it, itEnd = playableCards.end();
		for(it = playableCards.begin(); it != itEnd; ++it)
		{
			const HashString& card = it->first;
			wxInt32 count = it->second;

			for(wxInt32 i = 0; i < count; ++i)
			{
				// Get the actual name for the card.
				DataObject input(card), output;
				RULE.Decide(shLogicCardName, input, output);
				wxString name = output.read<wxString>();

				// Determine if the card is playable.
				output.reset();
				RULE.Decide(shLogicIsCardPlayable, input, output);

				bool playable = output.read<bool>();

                cards.push_back(
                    wxCardListCtrl::Card(name, card, 
                    playable ? wxColour(255, 255, 255) : wxColour(128, 128, 128),
                    playable));
			}
		}

        // Add all purchased cards.
		const Data::IntHash& purchasedCards = 
			playerGame.read<Data::IntHash>(shPurchasedCards);
		itEnd = purchasedCards.end();
		for(it = purchasedCards.begin(); it != itEnd; ++it)
		{
			const HashString& card = it->first;
			wxInt32 count = it->second;

			for(wxInt32 i = 0; i < count; ++i)
			{
				// Get the actual name for the card.
				DataObject input(card), output;
				RULE.Decide(shLogicCardName, input, output);
				const wxString& name = output.read<wxString>();

                cards.push_back(
                    wxCardListCtrl::Card(name, card, 
                    wxColour(128, 128, 128),
                    false));
			}
		}
	}
}

//---------------------------- PUBLIC           -----------------------------//
wxDevCardListCtrl::wxDevCardListCtrl(wxWindow* parent, wxWindowID id, 
	boost::weak_ptr<Game> game, const wxPoint& pos, const wxSize& size)
: wxCardListCtrl(parent, id, game, PopulateFunc(Populate), pos, size)
{
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
