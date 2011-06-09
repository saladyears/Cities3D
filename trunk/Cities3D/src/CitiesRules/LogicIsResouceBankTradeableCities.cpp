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
#include "Logic.h"
#include "RuleSetDatabase.h"
#include "CornerObject.h"
#include "HexObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicIsResourceBankTradeableCities
// 
// Determines whether any of the special cities rules are in effect that alter
// how resources or commodities can be traded to the bank.
//
// Derived From:
//     <Logic>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
// Mixin To:
//     <LogicIsResourceBankTradeable>
//
class LogicIsResourceBankTradeableCities : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		wxASSERT(2 <= input.numItems());

		bool canTrade = false;
		wxInt32 ratio = -1;

		const HashString& resource = input.read<HashString>();
		wxInt32 amount = input.read<wxInt32>(1);

		// If they have level 3 in trade, they have the Counting House upgrade,
		// which allows all commodities to be traded at a 2-1 ratio.
		if( (2 <= amount) &&
			(3 <= playerGameData<wxInt32>(shTradeLevel)) &&
			((shCloth == resource) ||
			(shPaper == resource) ||
			(shCoin == resource)))
		{
			canTrade = true;
			ratio = 2;
		}

		// If they have used the Merchant Fleet card, they are allowed to trade
		// the resource (or resources if they used more than 1) at 2-1.
		const HashStringArray& fleet = 
			playerGameData<HashStringArray>(shMerchantFleetCards);
		if(false == fleet.empty())
		{
			if( (2 <= amount) &&
				(fleet.end() != 
				std::find(fleet.begin(), fleet.end(), resource)))
			{
				canTrade = true;
				ratio = 2;
			}
		}

		// If the player has played the merchant, they can trade that resource
		// at 2-1.
		wxInt32 merchantOwner = gameData<wxInt32>(shMerchantPlayer);
		if(merchantOwner == current())
		{
			const HexObjectPtr& merchant = GetGame()->getHexObject(shMerchant);
			wxASSERT(merchant);

			wxInt32 tileNum = merchant->tile();
			const HashString& type = tile<HashString>(shTileType, tileNum);

			if( (2 <= amount) &&
				(resource == type))
			{
				canTrade = true;
				ratio = 2;
			}
		}
		
		output = DataObject(canTrade, ratio);
	}
};

IMPLEMENT_LOGIC_MIXIN(LogicIsResourceBankTradeableCities, 
					  LogicIsResourceBankTradeable, CITIES)
