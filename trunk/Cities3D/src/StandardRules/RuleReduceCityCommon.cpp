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
#include "Rule.h"
#include "RuleExtensionMixin.h"
#include "RuleSetDatabase.h"
#include "Controller.h"
#include "Utility.h"
#include "CornerObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleReduceCityCommon
// 
// Reduces the given city to a settlement.
//
// Derived From:
//     <Rule>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
//
class RuleReduceCityCommon 
	: public Rule
	, public RuleExtensionMixin
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(2 <= object.numItems());

		// Get the tile and corner where the city was reduced.
		wxInt32 tileId = object.read<wxInt32>();
		wxInt32 tile, corner;
		boost::tie(tile, corner) = Utility::decodeSel(tileId);
		wxInt32 player = object.read<wxInt32>(1);

		// Find the city in the player's current list.
		const PlayerGame::CornerObjectArray &cities = 
			playerGame(player).getCornerObjects(shCities);

		CornerObject::TileCorner tileCorner(tile, corner);

		CornerObjectPtr city;
		PlayerGame::CornerObjectArray::const_iterator it, 
			itEnd = cities.end();
		for(it = cities.begin(); it != itEnd; ++it)
		{
			if(true == (*it)->onTileCorner(tileCorner))
			{
				city = (*it);
				break;
			}
		}
		wxASSERT(city);
		wxInt32 id = city->id();

		// Play the city reduction sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_CITY_REDUCED));

		RULE.Execute(shRuleRemoveCity, object);
		ExecuteExtensions(DataObject(tileId, player, id));
		RULE.Execute(shRulePlaceSettlementReduced, object);
	}
};

IMPLEMENT_RULE(RuleReduceCityCommon, STANDARD)
