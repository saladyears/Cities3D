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
	void ProcessRulePlayCard(const DataObject& object, wxInt32& player, 
		wxInt32& adjust, HashString& modifier)
	{
		// Playing a dev card is always the current player.
		player = -2;
		adjust = 1;
		modifier = object.read<HashString>();
		modifier += shPlayedCard;
	}
}

//---------------------------- PUBLIC           -----------------------------//
// Player Stats.
IMPLEMENT_STAT(DEVCARDS, _("Development Cards Played"), "Development Cards Played", DevCardsPlayed, RulePlayCard, 
    StatPlayerFunc(boost::bind(ProcessRulePlayCard, _1, _2, _3, _4)), 70, false);

// Stat modifiers.
IMPLEMENT_STAT_MODIFIER(DEVCARDS, _("Monopoly"), "Monopoly", MonopolyPlayedCard, RuleNothing, DevCardsPlayed, 0);
IMPLEMENT_STAT_MODIFIER(DEVCARDS, _("Road Building"), "Road Building", RoadBuildingPlayedCard, RuleNothing, DevCardsPlayed, 10);
IMPLEMENT_STAT_MODIFIER(DEVCARDS, _("Soldier"), "Soldier", SoldierPlayedCard, RuleNothing, DevCardsPlayed, 20);
IMPLEMENT_STAT_MODIFIER(DEVCARDS, _("Victory Point"), "Victory Point", VictoryPointPlayedCard, RuleNothing, DevCardsPlayed, 30);
IMPLEMENT_STAT_MODIFIER(DEVCARDS, _("Year Of Plenty"), "Year Of Plenty", YearOfPlentyPlayedCard, RuleNothing, DevCardsPlayed, 40);

IMPLEMENT_MULTISTAT_MODIFIER_START(LargestArmy);
IMPLEMENT_MULTISTAT_MODIFIER_RULE(RuleGainLargestArmy);
IMPLEMENT_MULTISTAT_MODIFIER_RULE(RuleLuseLargestArmy);
IMPLEMENT_MULTISTAT_MODIFIER_END(LargestArmy, _("Largest Army"), "Largest Army", Points, DEVCARDS, 20);

IMPLEMENT_MULTISTAT_MODIFIER_START(VictoryPoints);
IMPLEMENT_MULTISTAT_MODIFIER_RULE(RulePlayCardVictoryPoint);
IMPLEMENT_MULTISTAT_MODIFIER_RULE(RuleWinDevCards);
IMPLEMENT_MULTISTAT_MODIFIER_END(VictoryPoints, _("Victory Points"), "Victory Points", Points, DEVCARDS, 30);

IMPLEMENT_STAT_MODIFIER(DEVCARDS, _("Monopoly"), "Monopoly", Monopoly, RuleExecuteMonopolyCard, ResourcesGainedFrom, 40);
IMPLEMENT_STAT_MODIFIER(DEVCARDS, _("Year Of Plenty"), "Year Of Plenty", YearOfPlenty, RuleExecuteYearOfPlentyCard, ResourcesGainedFrom, 50);

IMPLEMENT_STAT_MODIFIER(DEVCARDS, _("Monopoly"), "Monopoly", MonopolyLoss, RuleExecuteMonopolyCard, ResourcesLostTo, 50);

IMPLEMENT_STAT_MODIFIER(DEVCARDS, _("Development Cards"), "Development Cards", DevCardPurchased, RuleNothing, ItemsPurchased, 30);

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
