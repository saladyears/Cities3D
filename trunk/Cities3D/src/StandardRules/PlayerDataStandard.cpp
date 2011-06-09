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
#include "DrawRoadObject.h"
#include "DrawCityObject.h"
#include "DrawSettlementObject.h"
#include "RoadObject.h"
#include "CityObject.h"
#include "SettlementObject.h"
#include "SkinManager.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//---------------------------- STATICS          -----------------------------//
namespace
{
	wxInt32 updateResource(GamePtr game, const wxInt32 index, 
		const HashString &location, const HashString &type)
	{
		return game->playerGame(index).read(location, type);
	}

	wxInt32 updateResourceBack(GamePtr game, const wxInt32 index)
	{
		const Data::IntHash &hash(
			game->playerGame(index).read<Data::IntHash>(shResources));

		wxInt32 sum = 0;
		Data::IntHash::const_iterator it, itEnd(hash.end());
		for(it = hash.begin(); it != itEnd; ++it)
		{
			sum += it->second;
		}

		return sum;
	}

	wxColour colorResourceBack(GamePtr game, const wxInt32 index)
	{
		wxColour color = SKIN.Element(shPlayerUINeutral);

		//if they are over their maximum allowed cards, then the color
		//needs to turn on
		wxInt32 sum = updateResourceBack(game, index);

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

	wxColour colorPoints(GamePtr game, const wxInt32 index)
	{
		//for points, we turn them green if they are currently tied for, or
		//in the lead
		wxInt32 points = updateAmount(game, index, shPoints);

		bool lead = true;

		//now see if anyone is beating them
		for(wxInt32 i = 0; i < game->numPlayers(); i++)
		{
			if(updateAmount(game, i, shPoints) > points)
			{
				lead = false;
				break;
			}
		}

		HashString str = (lead ? shPlayerUIPositive : shPlayerUINeutral);

		//return green for the lead, white for no lead
		return SKIN.Element(str);
	}

	wxColour colorRoads(GamePtr game, const wxInt32 index)
	{
		wxColor color = SKIN.Element(shPlayerUINeutral);

		//for roads, turn them green if they have the longest road, or
		//white for normal
		if(index == game->read<wxInt32>(shLongestRoadPlayer))
		{
			color = SKIN.Element(shPlayerUIPositive);
		}

		return color;
	}
};

//player card resources
IMPLEMENT_PLAYER_IMAGE_DATA(STANDARD, PlayerResource, boost::bind(
							updateResource, _1, _2, shResources, 
							shSheep), boost::bind(
							colorResourceBack, _1, _2), 0, Resources, 
							Sheep, stSheep);
IMPLEMENT_PLAYER_IMAGE_DATA(STANDARD, PlayerResource, boost::bind(
							updateResource, _1, _2, shResources, 
							shOre), boost::bind(
							colorResourceBack, _1, _2), 0, Resources, 
							Ore, stOre);
IMPLEMENT_PLAYER_IMAGE_DATA(STANDARD, PlayerResource, boost::bind(
							updateResource, _1, _2, shResources, 
							shTimber), boost::bind(
							colorResourceBack, _1, _2), 0, Resources, 
							Timber, stTimber);
IMPLEMENT_PLAYER_IMAGE_DATA(STANDARD, PlayerResource, boost::bind(
							updateResource, _1, _2, shResources, 
							shClay), boost::bind(
							colorResourceBack, _1, _2), 0, Resources, 
							Clay, stClay);
IMPLEMENT_PLAYER_IMAGE_DATA(STANDARD, PlayerResource, boost::bind(
							updateResource, _1, _2, shResources, 
							shWheat), boost::bind(
							colorResourceBack, _1, _2), 0, Resources, 
							Wheat, stWheat);

//back of player card resource
IMPLEMENT_PLAYER_IMAGE_DATA(STANDARD, PlayerResourceBack, boost::bind(
							updateResourceBack, _1, _2), boost::bind(
							colorResourceBack, _1, _2), 0, Resources, Back, 
							stResourceCards);

//stock data
IMPLEMENT_PLAYER_OPENGL_DATA(STANDARD, PlayerStock, boost::bind(
							 updateAmount, _1, _2, shStockCities), 
							 PlayerColorFunc(), -1, CityObject, 
							 DrawCityObject, city32, no_mask,
							 stCities);
IMPLEMENT_PLAYER_OPENGL_DATA(STANDARD, PlayerStock, boost::bind(
							 updateAmount, _1, _2, shStockSettlements), 
							 PlayerColorFunc(), -1, SettlementObject, 
							 DrawSettlementObject, settlement32, no_mask,
							 stSettlements);
IMPLEMENT_PLAYER_OPENGL_DATA(STANDARD, PlayerStock, boost::bind(
							 updateAmount, _1, _2, shStockRoads), 
							 PlayerColorFunc(), -1, RoadObject, 
							 DrawRoadObject, road32, no_mask,
							 stRoads);

//points data
IMPLEMENT_PLAYER_IMAGE_DATA(STANDARD, PlayerPoints, boost::bind(
							updateAmount, _1, _2, shPoints), boost::bind(
							colorPoints, _1, _2), -1, Points, Points,
							stPoints);
IMPLEMENT_PLAYER_IMAGE_DATA(STANDARD, PlayerPoints, boost::bind(
							updateAmount, _1, _2, shVictoryPoints), 
							PlayerColorFunc(), 0, Points, VictoryPoints, 
							stVictoryPoints);
IMPLEMENT_PLAYER_IMAGE_DATA(STANDARD, PlayerPoints, boost::bind(
							updateAmount, _1, _2, shRoadLength), 
							boost::bind(colorRoads, _1, _2), 1, Points, Roads, 
							stRoadLength);

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
