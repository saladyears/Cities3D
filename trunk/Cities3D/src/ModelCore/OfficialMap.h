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
#pragma once

#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//
#include "Map.h"
#include "OfficialMapDatabase.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//
class OfficialMap : public Map
{
protected:
	OfficialMap()
	{
		mCreator = swJasonFugate;

		data<wxInt32>(shOfficial) = TRUE;

		mRuleSets.push_back(std::make_pair(swStandardRules, swStandard));
	}

	enum
	{
		MapStandard_III_IV					= 0,
		MapStandard_V_VI					,
		MapNewShores						,
		MapFourIslands_III					,
		MapFourIslands_IV					,
		MapFiveIslands_V					,
		MapSixIslands_VI					,
		MapOceans_III						,
		MapOceans_IV						,
		MapOceans_V							,
		MapOceans_VI						,
		MapIntoTheDesert_III				,
		MapIntoTheDesert_IV					,
		MapIntoTheDesert_V					,
		MapIntoTheDesert_VI					,
		MapNewWorld_III						,
		MapNewWorld_IV						,
		MapNewWorld_V						,
		MapNewWorld_VI						,
		MapGreatCrossing_IV					,
		MapGreatCrossing_VI					,
		MapGreaterCatan_IV					,
		MapGreaterCatan_V_VI				,
	};

	template <typename T> T& tile(const HashString &key, const wxInt32 tile)
	{
		return mTiles[tile].template data<T>(key);
	}
};

//---------------------------- PROTOTYPES       -----------------------------//

