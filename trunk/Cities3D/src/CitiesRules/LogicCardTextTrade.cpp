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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicCardTextTrade
// 
// Provides translated text of trade progress cards.
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
//     <LogicCardText>
//
class LogicCardTextCommercialHarbor : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stForEachOpponentOfferOne);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextCommercialHarbor, CommercialHarbor, 
							LogicCardText, CITIES);


// Master Merchant.

class LogicCardTextMasterMerchant : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stTakeTwoResources);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextMasterMerchant, MasterMerchant, 
							LogicCardText, CITIES);


// Merchant.

class LogicCardTextMerchant : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stSelectATileAdjacent);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextMerchant, Merchant, 
							LogicCardText, CITIES);

// Merchant Fleet.

class LogicCardTextMerchantFleet : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stSelectOneResourceOrCommodity);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextMerchantFleet, MerchantFleet, 
							LogicCardText, CITIES);

// Resource Monopoly.

class LogicCardTextResourceMonopoly : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stSelectAResource);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextResourceMonopoly, ResourceMonopoly, 
							LogicCardText, CITIES);

// Trade Monopoly.

class LogicCardTextTradeMonopoly : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stSelectACommodity);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardTextTradeMonopoly, TradeMonopoly, 
							LogicCardText, CITIES);

