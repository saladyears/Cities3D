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
#include "Game.h"
#include "DataObject.h"
#include "RuleSetDatabase.h"
#include "Stat.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	void ProcessRuleDiceRoll(const GamePtr& game, HashString& modifier)
	{
		wxInt32 roll = game->read<wxInt32>(shDie1) + game->read<wxInt32>(shDie2);
		modifier = HashString::Format(shIntFormat.c_str(), roll);
	}

	void ProcessRuleAdjustPoints(const DataObject& object, wxInt32& player, 
		wxInt32& adjust, HashString&)
	{
		player = object.read<wxInt32>();
		adjust = object.read<wxInt32>(1);
	}

	void ProcessRuleAdjustResourcesGained(const DataObject& object, wxInt32& player,
		wxInt32& adjust, HashString& modifier)
	{
        bool inUndo = RULE.InUndo();

		adjust = object.read<wxInt32>(1);

		// Only report for resources gained.
		if( ((0 < adjust) && (false == inUndo)) ||
            ((0 > adjust) && (true == inUndo)))
		{
			player = object.read<wxInt32>();
			modifier = object.read<HashString>(2);
		}
		else
		{
			modifier = shEmpty;
		}
	}

	void ProcessRuleAdjustResourcesLost(const DataObject& object, wxInt32& player,
		wxInt32& adjust, HashString& modifier)
	{
        bool inUndo = RULE.InUndo();

		adjust = object.read<wxInt32>(1);

		// Only report for resources lost, unless we're in an undo
		if( ((0 > adjust) && (false == inUndo)) ||
            ((0 < adjust) && (true == inUndo)))
		{
			player = object.read<wxInt32>();
			modifier = object.read<HashString>(2);

			// We show their losses as a positive number.
			adjust = -adjust;
		}
		else
		{
			modifier = shEmpty;
		}
	}

	void ProcessRuleBlockResources(const DataObject& object, wxInt32& player,
		wxInt32& adjust, HashString& modifier)
	{
		player = object.read<wxInt32>();
		adjust = object.read<wxInt32>(1);
		modifier = object.read<HashString>(2);
	}

    void ProcessRuleAdjustResourcesGainedFrom(const DataObject& object, wxInt32& player,
		wxInt32& adjust, HashString& modifier)
	{
        bool inUndo = RULE.InUndo();

		adjust = object.read<wxInt32>(1);

		// Only report for resources gained.
        if( ((0 < adjust) && (false == inUndo)) ||
            ((0 > adjust) && (true == inUndo)))
		{
			player = object.read<wxInt32>();
            wxASSERT(false == modifier.empty());
		}
        else
		{
			modifier = shEmpty;
		}
	}

	void ProcessRuleAdjustResourcesLostTo(const DataObject& object, wxInt32& player,
		wxInt32& adjust, HashString& modifier)
	{
        bool inUndo = RULE.InUndo();

		adjust = object.read<wxInt32>(1);

		// Only report for resources lost.
		if( ((0 > adjust) && (false == inUndo)) ||
            ((0 < adjust) && (true == inUndo)))
		{
			player = object.read<wxInt32>();

			// We show their losses as a positive number.
			adjust = -adjust;

            wxASSERT(false == modifier.empty());
		}
		else
		{
			modifier = shEmpty;
		}
	}

	void ProcessRuleBlockResourcesBy(const DataObject& object, wxInt32& player,
		wxInt32& adjust, HashString& modifier)
	{
		player = object.read<wxInt32>();
		adjust = object.read<wxInt32>(1);
		
		wxASSERT(false == modifier.empty());
	}

	void ProcessRulePurchaseItem(const DataObject& object, wxInt32& player,
		wxInt32& adjust, HashString& modifier)
	{
		player = object.read<wxInt32>();
		modifier = object.read<HashString>(1);
		modifier += shPurchased;

		if(true == RULE.InUndo())
		{
			adjust = -1;
		}
		else
		{
			adjust = 1;
		}
	}

	void ProcessRuleTakeAction(const DataObject& object, wxInt32& player,
		wxInt32& adjust, HashString& modifier)
	{
		player = object.read<wxInt32>();
		modifier = object.read<HashString>(1);
		modifier += shAction;

		if(true == RULE.InUndo())
		{
			adjust = -1;
		}
		else
		{
			adjust = 1;
		}
	}
}

//---------------------------- PUBLIC           -----------------------------//

// Game Stats.
IMPLEMENT_STAT(STANDARD, _("Dice Rolls"), "Dice Rolls", DiceRolls, RuleBeginTurn,
	StatDiceRollFunc(boost::bind(ProcessRuleDiceRoll, _1, _2)), 0, false)

// Player Stats.
IMPLEMENT_STAT(STANDARD, _("Points"), "Points", Points, RuleAdjustPoints, 
	StatPlayerFunc(boost::bind(ProcessRuleAdjustPoints, _1, _2, _3, _4)), 10, false);

IMPLEMENT_STAT(STANDARD, _("Resources Gained"), "Resources Gained", ResourcesGained, RuleAdjustResources,
	StatPlayerFunc(boost::bind(ProcessRuleAdjustResourcesGained, _1, _2, _3, _4)), 20, true);

IMPLEMENT_STAT(STANDARD, _("Resources Lost"), "Resources Lost", ResourcesLost, RuleAdjustResources,
	StatPlayerFunc(boost::bind(ProcessRuleAdjustResourcesLost, _1, _2, _3, _4)), 30, true);

IMPLEMENT_STAT(STANDARD, _("Resources Blocked"), "Resources Blocked", ResourcesBlocked, RuleBlockResources,
	StatPlayerFunc(boost::bind(ProcessRuleBlockResources, _1, _2, _3, _4)), 40, true);

IMPLEMENT_STAT(STANDARD, _("Resources Gained From"), "Resources Gained From", ResourcesGainedFrom, RuleAdjustResources,
	StatPlayerFunc(boost::bind(ProcessRuleAdjustResourcesGainedFrom, _1, _2, _3, _4)), 25, false);

IMPLEMENT_STAT(STANDARD, _("Resources Lost To"), "Resources Lost To", ResourcesLostTo, RuleAdjustResources,
	StatPlayerFunc(boost::bind(ProcessRuleAdjustResourcesLostTo, _1, _2, _3, _4)), 35, false);

IMPLEMENT_STAT(STANDARD, _("Resources Blocked By"), "Resources Blocked By", ResourcesBlockedBy, RuleBlockResources,
	StatPlayerFunc(boost::bind(ProcessRuleBlockResourcesBy, _1, _2, _3, _4)), 45, false);

IMPLEMENT_STAT(STANDARD, _("Purchases Made"), "Purchases Made", ItemsPurchased, RulePurchaseItem,
	StatPlayerFunc(boost::bind(ProcessRulePurchaseItem, _1, _2, _3, _4)), 50, false);

IMPLEMENT_STAT(STANDARD, _("Actions Taken"), "Actions Taken", ActionsTaken, RuleTakeAction,
	StatPlayerFunc(boost::bind(ProcessRuleTakeAction, _1, _2, _3, _4)), 60, false);

// Game Stat Modifiers.
IMPLEMENT_DICE_ROLL_STAT_MODIFIER(STANDARD, wxT("2"), "2", 2, RuleNothing, DiceRolls, 0, 1.0f/36.0f);
IMPLEMENT_DICE_ROLL_STAT_MODIFIER(STANDARD, wxT("3"), "3", 3, RuleNothing, DiceRolls, 10, 2.0f/36.0f);
IMPLEMENT_DICE_ROLL_STAT_MODIFIER(STANDARD, wxT("4"), "4", 4, RuleNothing, DiceRolls, 20, 3.0f/36.0f);
IMPLEMENT_DICE_ROLL_STAT_MODIFIER(STANDARD, wxT("5"), "5", 5, RuleNothing, DiceRolls, 30, 4.0f/36.0f);
IMPLEMENT_DICE_ROLL_STAT_MODIFIER(STANDARD, wxT("6"), "6", 6, RuleNothing, DiceRolls, 40, 5.0f/36.0f);
IMPLEMENT_DICE_ROLL_STAT_MODIFIER(STANDARD, wxT("7"), "7", 7, RuleNothing, DiceRolls, 50, 6.0f/36.0f);
IMPLEMENT_DICE_ROLL_STAT_MODIFIER(STANDARD, wxT("8"), "8", 8, RuleNothing, DiceRolls, 60, 5.0f/36.0f);
IMPLEMENT_DICE_ROLL_STAT_MODIFIER(STANDARD, wxT("9"), "9", 9, RuleNothing, DiceRolls, 70, 4.0f/36.0f);
IMPLEMENT_DICE_ROLL_STAT_MODIFIER(STANDARD, wxT("10"), "10", 10, RuleNothing, DiceRolls, 80, 3.0f/36.0f);
IMPLEMENT_DICE_ROLL_STAT_MODIFIER(STANDARD, wxT("11"), "11", 11, RuleNothing, DiceRolls, 90, 2.0f/36.0f);
IMPLEMENT_DICE_ROLL_STAT_MODIFIER(STANDARD, wxT("12"), "12", 12, RuleNothing, DiceRolls, 100, 1.0f/36.0f);

// Player Stat Modifiers.
IMPLEMENT_MULTISTAT_MODIFIER_START(Structures);
IMPLEMENT_MULTISTAT_MODIFIER_RULE(RulePlaceCity);
IMPLEMENT_MULTISTAT_MODIFIER_RULE(RulePlaceSettlementCommon);
IMPLEMENT_MULTISTAT_MODIFIER_RULE(RuleRemoveCity);
IMPLEMENT_MULTISTAT_MODIFIER_RULE(RuleRemoveSettlement);
IMPLEMENT_MULTISTAT_MODIFIER_END(Structures, _("Structures"), "Structures", Points, STANDARD, 0);

IMPLEMENT_MULTISTAT_MODIFIER_START(LongestRoad);
IMPLEMENT_MULTISTAT_MODIFIER_RULE(RuleGainLongestRoad);
IMPLEMENT_MULTISTAT_MODIFIER_RULE(RuleLoseLongestRoad);
IMPLEMENT_MULTISTAT_MODIFIER_END(LongestRoad, _("Longest Road"), "Longest Road", Points, STANDARD, 10);

IMPLEMENT_STAT_MODIFIER(STANDARD, _("Sheep"), "Sheep", Sheep, RuleNothing, ResourcesGained, 0);
IMPLEMENT_STAT_MODIFIER(STANDARD, _("Ore"), "Ore", Ore, RuleNothing, ResourcesGained, 10);
IMPLEMENT_STAT_MODIFIER(STANDARD, _("Timber"), "Timber", Timber, RuleNothing, ResourcesGained, 20);
IMPLEMENT_STAT_MODIFIER(STANDARD, _("Clay"), "Clay", Clay, RuleNothing, ResourcesGained, 30);
IMPLEMENT_STAT_MODIFIER(STANDARD, _("Wheat"), "Wheat", Wheat, RuleNothing, ResourcesGained, 40);

IMPLEMENT_STAT_MODIFIER(STANDARD, _("Sheep"), "Sheep", Sheep, RuleNothing, ResourcesLost, 0);
IMPLEMENT_STAT_MODIFIER(STANDARD, _("Ore"), "Ore", Ore, RuleNothing, ResourcesLost, 10);
IMPLEMENT_STAT_MODIFIER(STANDARD, _("Timber"), "Timber", Timber, RuleNothing, ResourcesLost, 20);
IMPLEMENT_STAT_MODIFIER(STANDARD, _("Clay"), "Clay", Clay, RuleNothing, ResourcesLost, 30);
IMPLEMENT_STAT_MODIFIER(STANDARD, _("Wheat"), "Wheat", Wheat, RuleNothing, ResourcesLost, 40);

IMPLEMENT_STAT_MODIFIER(STANDARD, _("Sheep"), "Sheep", Sheep, RuleNothing, ResourcesBlocked, 0);
IMPLEMENT_STAT_MODIFIER(STANDARD, _("Ore"), "Ore", Ore, RuleNothing, ResourcesBlocked, 10);
IMPLEMENT_STAT_MODIFIER(STANDARD, _("Timber"), "Timber", Timber, RuleNothing, ResourcesBlocked, 20);
IMPLEMENT_STAT_MODIFIER(STANDARD, _("Clay"), "Clay", Clay, RuleNothing, ResourcesBlocked, 30);
IMPLEMENT_STAT_MODIFIER(STANDARD, _("Wheat"), "Wheat", Wheat, RuleNothing, ResourcesBlocked, 40);

IMPLEMENT_MULTISTAT_MODIFIER_START(Board);
IMPLEMENT_MULTISTAT_MODIFIER_RULE(RuleGatherResources);
IMPLEMENT_MULTISTAT_MODIFIER_RULE(RuleInitialResources);
IMPLEMENT_MULTISTAT_MODIFIER_END(Board, _("Board"), "Board", ResourcesGainedFrom, STANDARD, 0);

IMPLEMENT_STAT_MODIFIER(STANDARD, _("Robber"), "Robber", Robber, RuleExecuteRobberSteal, ResourcesGainedFrom, 10);
IMPLEMENT_STAT_MODIFIER(STANDARD, _("Bank Trades"), "Bank Trades", BankTrades, RuleExecuteBankTrade, ResourcesGainedFrom, 20);
IMPLEMENT_STAT_MODIFIER(STANDARD, _("Player Trades"), "Player Trades", PlayerTrades, RuleExecuteTrade, ResourcesGainedFrom, 30);

IMPLEMENT_STAT_MODIFIER(STANDARD, _("Purchases"), "Purchases", Purchases, RulePurchase, ResourcesLostTo, 0);
IMPLEMENT_STAT_MODIFIER(STANDARD, _("7s"), "7s", Sevens, RuleExecuteLose7, ResourcesLostTo, 10);
IMPLEMENT_STAT_MODIFIER(STANDARD, _("Robber"), "Robber", RobberLoss, RuleExecuteRobberSteal, ResourcesLostTo, 20);
IMPLEMENT_STAT_MODIFIER(STANDARD, _("Bank Trades"), "Bank Trades", BankTradesLoss, RuleExecuteBankTrade, ResourcesLostTo, 30);
IMPLEMENT_STAT_MODIFIER(STANDARD, _("Player Trades"), "Player Trades", PlayerTradesLoss, RuleExecuteTrade, ResourcesLostTo, 40);

IMPLEMENT_STAT_MODIFIER(STANDARD, _("Robber"), "Robber", RobberBlock, RuleGatherResources, ResourcesBlockedBy, 0);
IMPLEMENT_STAT_MODIFIER(STANDARD, _("Empty Bank"), "Empty Bank", EmptyBank, RuleBlockBankResources, ResourcesBlockedBy, 10);

IMPLEMENT_STAT_MODIFIER(STANDARD, _("Roads"), "Roads", RoadPurchased, RuleNothing, ItemsPurchased, 0);
IMPLEMENT_STAT_MODIFIER(STANDARD, _("Settlements"), "Settlements", SettlementPurchased, RuleNothing, ItemsPurchased, 10);
IMPLEMENT_STAT_MODIFIER(STANDARD, _("Cities"), "Cities", CityPurchased, RuleNothing, ItemsPurchased, 20);

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
