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
#include "DiscardProgressCardListCtrl.h"
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
	struct PopulateFunctionDiscard
	{
		PopulateFunctionDiscard(wxInt32 player)
			: mPlayer(player)
		{}

		void PopulateType(const HashString& type, wxColour color, 
			const GamePtr& game, wxInt32 player, 
			wxCardListCtrl::CardArray& cards)
		{
			// Now that we have the player, get their game data.
			wxASSERT(-1 != player);
			const PlayerGame& playerGame = game->playerGame(player);
			const Data::IntHash& playableCards = 
				playerGame.read<Data::IntHash>(type);

			// Add all cards as enabled.
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
					const wxString& name = output.read<wxString>();

                    cards.push_back(wxCardListCtrl::Card(name, card, color, true));
				}
			}
		}

		void operator()(const GamePtr& game, wxInt32, wxCardListCtrl::CardArray& cards)
		{
			PopulateType(shPlayableTradeCards, wxColour(255, 255, 128), game, 
                mPlayer, cards);
			PopulateType(shPlayablePoliticsCards, wxColour(0, 128, 255), game, 
                mPlayer, cards);
			PopulateType(shPlayableScienceCards, wxColour(128, 255, 128), game, 
                mPlayer, cards);
		}

		wxInt32 mPlayer;
	};
}

//---------------------------- PUBLIC           -----------------------------//
wxDiscardProgressCardListCtrl::wxDiscardProgressCardListCtrl(wxWindow* parent, 
	wxWindowID id, boost::weak_ptr<Game> game, wxInt32 player, 
	const wxPoint& pos, const wxSize& size)
	: wxCardListCtrl(parent, id, game, PopulateFunctionDiscard(player), pos, size)
{
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
