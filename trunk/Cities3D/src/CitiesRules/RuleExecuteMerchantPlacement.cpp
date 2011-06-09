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
#include "RuleSetDatabase.h"
#include "Controller.h"
#include "IRuleEngine.h"
#include "HexObject.h"
#include "CornerObject.h"
#include "UtilityGL.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleExecuteMerchantPlacement
// 
// Places the merchant at the given tile.
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
class RuleExecuteMerchantPlacement : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
        // The current player has ended a blocking action.
        RULE.Execute(shRuleEndBlockingAction, DataObject(current()));

		// Clear out all tile flags.
		RULE.Execute(shRuleResetTileFlags, DataObject());

		// Get the tile being set.
		wxInt32 thisTile = object.read<wxInt32>();

		HexObjectPtr merchant = GetGame()->getHexObject(shMerchant);
		wxASSERT(merchant);

		merchant->reset();
		merchant->tile(thisTile);

		Vector coords = tile(thisTile)->coords();
		wxInt32 corner = FindCorner(thisTile);
		UtilityGL::cornerCoords(coords, corner, 0.60f);
		merchant->coords(coords);

		Controller::get().Transmit(shEventDirty, 0);

		// Play the sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_PLACE_MERCHANT));

		// The merchant adds one point to the new player and takes one point
		// away from any old owner.
		wxInt32 oldOwner = gameData<wxInt32>(shMerchantPlayer);

        // Set the new owner now so when the old owner's update happens, they
        // aren't still the owner.
        wxInt32 newOwner = current();
		gameData<wxInt32>(shMerchantPlayer) = newOwner;
		
		if(-1 != oldOwner)
		{
			RULE.Execute(shRuleAdjustPoints, DataObject(oldOwner, -1));
			Controller::get().Transmit(shEventPlayerUI, 
				DataObject(GetGame(), oldOwner));
		}

		RULE.Execute(shRuleAdjustPoints, DataObject(newOwner, 1));
		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), newOwner));

		// The merchant allows 2-1 trading, so the bank must be updated.
		Controller::get().Transmit(shEventControlsUI, DataObject(true));

		RULE.Execute(shRulePlayProgressCardEnd, DataObject());
	}

private:
	wxInt32 FindCorner(wxInt32 tileNum)
	{
		// Find the first object this player owns on this tile.
		wxInt32 corner = 0;

		PlayerGame::CornerObjectArray objects = 
			playerGame().getCornerObjects(shSettlements);
		const PlayerGame::CornerObjectArray& cities =
			playerGame().getCornerObjects(shCities);
		std::copy(cities.begin(), cities.end(), std::back_inserter(objects));

		CornerObject::TileCorner tile1, tile2, tile3;
		PlayerGame::CornerObjectArray::const_iterator it, 
			itEnd = objects.end();
		for(it = objects.begin(); it != itEnd; ++it)
		{
			const CornerObjectPtr& object = (*it);
			tile1 = object->tile1();
			tile2 = object->tile2();
			tile3 = object->tile3();

			if(tile1.first == tileNum)
			{
				corner = tile1.second;
				break;
			}
			if(tile2.first == tileNum)
			{
				corner = tile2.second;
				break;
			}
			if(tile3.first == tileNum)
			{
				corner = tile3.second;
				break;
			}
		}

		return corner;
	}
};

IMPLEMENT_RULE(RuleExecuteMerchantPlacement, CITIES)
