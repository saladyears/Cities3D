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
// Class: LogicCardNameTrade
// 
// Provides translated names of trade progress cards.
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
//     <LogicCardName>
//
class LogicCardNameCommercialHarbor : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stCommercialHarbor);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardNameCommercialHarbor, CommercialHarbor, 
							LogicCardName, CITIES);


// Master Merchant.

class LogicCardNameMasterMerchant : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stMasterMerchant);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardNameMasterMerchant, MasterMerchant, 
							LogicCardName, CITIES);


// Merchant.

class LogicCardNameMerchant : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stMerchant);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardNameMerchant, Merchant, 
							LogicCardName, CITIES);

// Merchant Fleet.

class LogicCardNameMerchantFleet : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stMerchantFleet);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardNameMerchantFleet, MerchantFleet, 
							LogicCardName, CITIES);

// Resource Monopoly.

class LogicCardNameResourceMonopoly : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stResourceMonopoly);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardNameResourceMonopoly, ResourceMonopoly, 
							LogicCardName, CITIES);

// Trade Monopoly.

class LogicCardNameTradeMonopoly : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		output = DataObject(stTradeMonopoly);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicCardNameTradeMonopoly, TradeMonopoly, 
							LogicCardName, CITIES);

