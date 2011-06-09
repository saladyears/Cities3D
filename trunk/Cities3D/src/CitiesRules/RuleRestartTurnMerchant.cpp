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
#include "DummySelectionObject.h"
#include "HexObject.h"
#include "CornerObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRestartTurnMerchant
// 
// Starts up the player placing the merchant.
//
// Derived From:
//     <Rule>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
class RuleRestartTurnMerchant : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
        // The current player is now in a blocking action.
        RULE.Execute(shRuleBeginBlockingAction, DataObject(current()));

		RULE.Execute(shRuleUpdateMessageUI, DataObject(stPlaceTheMerchant, 
            stWaitingPlaceTheMerchant));

		//set the dummy selection
		DummySelectionObject *pSelection = new DummySelectionObject(
			shRuleExecuteMerchantPlacement);

		//send the dummy selection object up
		Controller::get().Transmit(shEventSelectionObject, 
			SelectionObjectPtr(pSelection));

		// Make sure eveything is hidden.
		Controller::get().Transmit(shEventBuildUI, 
			DataObject(false, GetGame()));
		Controller::get().Transmit(shEventControlsUI, DataObject(false));

		RULE.Execute(shRuleResetTileFlags, DataObject(IGameObject::Dim));

		// Hide the current merchant until the new one is placed
		const HexObjectPtr& merchant = GetGame()->getHexObject(shMerchant);
		wxASSERT(merchant);
		
		// If this player is placing the merchant again, they are not allowed
		// to place the merchant on the same type of tile as the last time.
		HashString avoid;
		if(current() == gameData<wxInt32>(shMerchantPlayer))
		{
			wxInt32 tileAvoid = merchant->tile();
			avoid = tile<HashString>(shTileType, tileAvoid);
		}

		merchant->set(IGameObject::Hidden);

		// They can place the merchant on any resource tile that borders one
		// of their cities or settlements.

		PlayerGame::CornerObjectArray objects = 
			playerGame().getCornerObjects(shSettlements);
		const PlayerGame::CornerObjectArray& cities = 
			playerGame().getCornerObjects(shCities);
		std::copy(cities.begin(), cities.end(), std::back_inserter(objects));

		wxInt32 tile1, tile2, tile3;

		PlayerGame::CornerObjectArray::const_iterator it,
			itEnd = objects.end();
		for(it = objects.begin(); it != itEnd; ++it)
		{
			const CornerObjectPtr& obj = (*it);

			// Turn on all resource tiles next to this player's objects.
			tile1 = obj->tile1().first;
			tile2 = obj->tile2().first;
			tile3 = obj->tile3().first;

			SetCorner(tile1, avoid);
			SetCorner(tile2, avoid);
			SetCorner(tile3, avoid);
		}
	}

private:
	void SetCorner(wxInt32 index, const HashString& avoid)
	{
		if(-1 != index)
		{
			DataObject input(index), output;
			RULE.Decide(shLogicTileIsResource, input, output);

			if( (true == output.read<bool>()) &&
				(avoid != tile<HashString>(shTileType, index)))
			{
				tile(index)->reset();
				tile(index)->set(IGameObject::Select);
			}
		}
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RuleRestartTurnMerchant, TurnMerchant, 
						   RuleRestartGame, CITIES)
