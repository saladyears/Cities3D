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
#include "SkinManager.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	wxInt32 updateCardBack(GamePtr game, const wxInt32 index)
	{
		//we need to sum up their purchased cards and their playable cards
		const Data::IntHash &hash(game->playerGame(index).read<Data::IntHash>(
			shPurchasedCards));

		wxInt32 sum = 0;
		Data::IntHash::const_iterator it, itEnd(hash.end());
		for(it = hash.begin(); it != itEnd; ++it)
		{
			sum += it->second;
		}

		//playable cards
		const Data::IntHash &hash2(game->playerGame(index).read<Data::IntHash>(
			shPlayableCards));

		itEnd = hash2.end();
		for(it = hash2.begin(); it != itEnd; ++it)
		{
			sum += it->second;
		}

		return sum;
	}

	wxInt32 updateSoldier(GamePtr game, const wxInt32 index)
	{
		return game->playerGame(index).read(shPlayedCards, 
			shSoldier);
	}

	wxColor colorSoldier(GamePtr game, const wxInt32 index)
	{
		wxColour color = SKIN.Element(shPlayerUINeutral);

		//they get green if they have largest army, otherwise, white
		if(index == game->read<wxInt32>(shLargestArmyPlayer))
		{
			color = SKIN.Element(shPlayerUIPositive);
		}

		return color;
	}
};

//back of development card
IMPLEMENT_PLAYER_IMAGE_DATA(DEVCARDS, PlayerCardBack, boost::bind(
							updateCardBack, _1, _2), PlayerColorFunc(), 0, 
							Cards, DevCard, stDevelopmentCards);

//points data
IMPLEMENT_PLAYER_IMAGE_DATA(DEVCARDS, PlayerPoints, boost::bind(
							updateSoldier, _1, _2), boost::bind(
							colorSoldier, _1, _2), 0, Points, Soldier, 
							stSoldiers);

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//


