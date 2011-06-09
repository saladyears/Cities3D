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


#include "PlayerData.h"
#include "RuleSetDatabase.h"
#include "Game.h"
#include "DrawWallsObject.h"
#include "DrawKnightObject.h"
#include "WallsObject.h"
#include "KnightObject.h"
#include "SkinManager.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//---------------------------- STATICS          -----------------------------//
namespace
{
	wxInt32 updateCardBack(GamePtr game, const wxInt32 index, 
		const HashString &type)
	{
		//we need to sum up their purchased cards
		const Data::IntHash &hash(
			game->playerGame(index).read<Data::IntHash>(type));

		wxInt32 sum = 0;
		Data::IntHash::const_iterator it, itEnd(hash.end());
		for(it = hash.begin(); it != itEnd; ++it)
		{
			sum += it->second;
		}

		return sum;
	}

	wxInt32 updateResource(GamePtr game, const wxInt32 index, 
		const HashString &location, const HashString &type)
	{
		return game->playerGame(index).read(location, type);
	}

	wxColour colorResourceBack(GamePtr game, const wxInt32 index)
	{
		wxColour color = SKIN.Element(shPlayerUINeutral);

		//if they are over their maximum allowed cards, then the color
		//needs to turn on
		wxInt32 sum = updateCardBack(game, index, shResources);

		//see if the go over (into the red zone)
		if(sum > game->playerGame(index).read<wxInt32>(shAllowedCards))
		{
			color = SKIN.Element(shPlayerUINegative);
		}

		return color;
	}

	wxInt32 updateAmount(GamePtr game, const wxInt32 index, 
		const HashString &value)
	{
		return game->playerGame(index).read<wxInt32>(value);
	}

	wxInt32 updatePlayerIcon(GamePtr game, const wxInt32 index,
		const HashString& type)
	{
		wxBOOL ret = (index == game->read<wxInt32>(type));

		return ret;
	}
};

//player card resources
IMPLEMENT_PLAYER_IMAGE_DATA(CITIES, PlayerResource, boost::bind(
							updateResource, _1, _2, shResources, 
							shCloth), boost::bind(
							colorResourceBack, _1, _2), 0, Resources, 
							Cloth, stCloth);
IMPLEMENT_PLAYER_IMAGE_DATA(CITIES, PlayerResource, boost::bind(
							updateResource, _1, _2, shResources, 
							shCoin), boost::bind(
							colorResourceBack, _1, _2), 0, Resources, 
							Coin, stCoin);
IMPLEMENT_PLAYER_IMAGE_DATA(CITIES, PlayerResource, boost::bind(
							updateResource, _1, _2, shResources, 
							shPaper), boost::bind(
							colorResourceBack, _1, _2), 0, Resources, 
							Paper, stPaper);

//progress cards
IMPLEMENT_PLAYER_IMAGE_DATA(CITIES, PlayerCardBack, boost::bind(
							updateCardBack, _1, _2, 
							shPlayableTradeCards), PlayerColorFunc(), 
							0, Cards, TradeCard, stTradeCards);
IMPLEMENT_PLAYER_IMAGE_DATA(CITIES, PlayerCardBack, boost::bind(
							updateCardBack, _1, _2, 
							shPlayablePoliticsCards), PlayerColorFunc(), 
							0, Cards, PoliticsCard, stPoliticsCards);
IMPLEMENT_PLAYER_IMAGE_DATA(CITIES, PlayerCardBack, boost::bind(
							updateCardBack, _1, _2, 
							shPlayableScienceCards), PlayerColorFunc(), 
							0, Cards, ScienceCard, stScienceCards);

//stock data
IMPLEMENT_PLAYER_OPENGL_DATA(CITIES, PlayerStock, boost::bind(
							 updateAmount, _1, _2, shStockWalls), 
							 PlayerColorFunc(), -1, WallsObject, 
							 DrawWallsObject, walls32, no_mask,
							 stCityWalls);
IMPLEMENT_PLAYER_OPENGL_DATA(CITIES, PlayerStock, boost::bind(
							 updateAmount, _1, _2, shStockKnights1), 
							 PlayerColorFunc(), -1, KnightObject, 
							 DrawKnightObject, knight132, knight32_mask,
							 stBasicKnights);
IMPLEMENT_PLAYER_OPENGL_DATA(CITIES, PlayerStock, boost::bind(
							 updateAmount, _1, _2, shStockKnights2), 
							 PlayerColorFunc(), -1, Knight2Object, 
							 DrawKnightObject, knight232, knight32_mask,
							 stStrongKnights);
IMPLEMENT_PLAYER_OPENGL_DATA(CITIES, PlayerStock, boost::bind(
							 updateAmount, _1, _2, shStockKnights3), 
							 PlayerColorFunc(), -1, Knight3Object, 
							 DrawKnightObject, knight332, knight32_mask,
							 stMightyKnights);

//points data
IMPLEMENT_PLAYER_IMAGE_DATA(CITIES, PlayerPoints, boost::bind(
							updateAmount, _1, _2, shDefenderPoints), 
							PlayerColorFunc(), 0, Points, DefenderPoints,
							stDefenderPoints);

// Player icons.
IMPLEMENT_PLAYER_IMAGE_DATA(CITIES, PlayerSpecial, boost::bind(updatePlayerIcon,
							_1, _2, shSciencePlayer), PlayerColorFunc(), -1,
							PlayerIcon, Science, stScienceMetropolis);
IMPLEMENT_PLAYER_IMAGE_DATA(CITIES, PlayerSpecial, boost::bind(updatePlayerIcon,
							_1, _2, shPoliticsPlayer), PlayerColorFunc(), -1,
							PlayerIcon, Politics, stPoliticsMetropolis);
IMPLEMENT_PLAYER_IMAGE_DATA(CITIES, PlayerSpecial, boost::bind(updatePlayerIcon,
							_1, _2, shTradePlayer), PlayerColorFunc(), -1,
							PlayerIcon, Trade, stTradeMetropolis);
IMPLEMENT_PLAYER_IMAGE_DATA(CITIES, PlayerSpecial, boost::bind(updatePlayerIcon,
							_1, _2, shMerchantPlayer), PlayerColorFunc(), -1,
							PlayerIcon, Merchant, stMerchant);

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
