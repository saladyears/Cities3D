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
	WX_DECLARE_HASH_STRING_HASH_SET(StringSet);

	StringSet sTradePlayedCards;
	StringSet sPoliticsPlayedCards;
	StringSet sSciencePlayedCards;

	void ProcessRuleRedDie(const GamePtr& game, HashString& modifier)
	{
		wxInt32 roll = game->read<wxInt32>(shDie2);
		modifier = HashString::Format(shRInt.c_str(), roll);
	}

	void ProcessRuleThirdDie(const GamePtr& game, HashString& modifier)
	{
		wxInt32 roll = game->read<wxInt32>(shDie3);

		switch(roll)
		{
		case 1:
		case 2:
		case 3:
			modifier = shBarbarian;
			break;
		case 4:
			modifier = shTrade;
			break;
		case 5:
			modifier = shPolitics;
			break;
		case 6:
			modifier = shScience;
			break;
		default:
			wxASSERT(false);
			break;
		}
	}

	void ProcessRulePlayCardTrade(const DataObject& object, wxInt32& player, 
		wxInt32& adjust, HashString& modifier)
	{
		if(true == sTradePlayedCards.empty())
		{
			sTradePlayedCards.insert(shCommercialHarbor);
			sTradePlayedCards.insert(shMasterMerchant);
			sTradePlayedCards.insert(shMerchant);
			sTradePlayedCards.insert(shMerchantFleet);
			sTradePlayedCards.insert(shResourceMonopoly);
			sTradePlayedCards.insert(shTradeMonopoly);
		}

		const HashString& card = object.read<HashString>();
		if(sTradePlayedCards.end() != sTradePlayedCards.find(card))
		{
			player = -2;
			adjust = 1;
			modifier = card;
			modifier += shPlayedCard;
		}
	}

	void ProcessRulePlayCardPolitics(const DataObject& object, wxInt32& player, 
		wxInt32& adjust, HashString& modifier)
	{
		if(true == sPoliticsPlayedCards.empty())
		{
			sPoliticsPlayedCards.insert(shArsonist);
			sPoliticsPlayedCards.insert(shBishop);
			sPoliticsPlayedCards.insert(shConstitution);
			sPoliticsPlayedCards.insert(shDeserter);
			sPoliticsPlayedCards.insert(shDiplomat);
			sPoliticsPlayedCards.insert(shIntrigue);
			sPoliticsPlayedCards.insert(shSaboteur);
			sPoliticsPlayedCards.insert(shSpy);
			sPoliticsPlayedCards.insert(shWarlord);
			sPoliticsPlayedCards.insert(shWedding);
		}

		const HashString& card = object.read<HashString>();
		if(sPoliticsPlayedCards.end() != sPoliticsPlayedCards.find(card))
		{
			player = -2;
			adjust = 1;
			modifier = card;
			modifier += shPlayedCard;
		}
	}

	void ProcessRulePlayCardScience(const DataObject& object, wxInt32& player, 
		wxInt32& adjust, HashString& modifier)
	{
		if(true == sSciencePlayedCards.empty())
		{
			sSciencePlayedCards.insert(shAlchemist);
			sSciencePlayedCards.insert(shCrane);
			sSciencePlayedCards.insert(shEngineer);
			sSciencePlayedCards.insert(shInventor);
			sSciencePlayedCards.insert(shIrrigation);
			sSciencePlayedCards.insert(shMedicine);
			sSciencePlayedCards.insert(shMining);
			sSciencePlayedCards.insert(shPrinter);
			sSciencePlayedCards.insert(shRoadBuilding);
			sSciencePlayedCards.insert(shSmith);
		}

		const HashString& card = object.read<HashString>();
		if(sSciencePlayedCards.end() != sSciencePlayedCards.find(card))
		{
			player = -2;
			adjust = 1;
			modifier = card;
			modifier += shPlayedCard;
		}
	}

}

//---------------------------- PUBLIC           -----------------------------//
IMPLEMENT_STAT(CITIES, _("Red Die"), "Red Die", RedDie, RuleBeginTurn, 
	StatDiceRollFunc(boost::bind(ProcessRuleRedDie, _1, _2)), 1, false);

IMPLEMENT_STAT(CITIES, _("Third Die"), "Third Die", ThirdDie, RuleBeginTurn, 
	StatDiceRollFunc(boost::bind(ProcessRuleThirdDie, _1, _2)), 2, false);

IMPLEMENT_STAT(CITIES, _("Trade Cards Played"), "Trade Cards Played", TradeCardsPlayed, RulePlayCard,
	StatPlayerFunc(boost::bind(ProcessRulePlayCardTrade, _1, _2, _3, _4)), 70, false);

IMPLEMENT_STAT(CITIES, _("Politics Cards Played"), "Politics Cards Played", PoliticsCardsPlayed, RulePlayCard,
	StatPlayerFunc(boost::bind(ProcessRulePlayCardPolitics, _1, _2, _3, _4)), 80, false);

IMPLEMENT_STAT(CITIES, _("Science Cards Played"), "Science Cards Played", ScienceCardsPlayed, RulePlayCard,
	StatPlayerFunc(boost::bind(ProcessRulePlayCardScience, _1, _2, _3, _4)), 90, false);

IMPLEMENT_DICE_ROLL_STAT_MODIFIER(CITIES, wxT("1"), "1", R1, RuleNothing, RedDie, 0, 1.0f/6.0f);
IMPLEMENT_DICE_ROLL_STAT_MODIFIER(CITIES, wxT("2"), "2", R2, RuleNothing, RedDie, 10, 1.0f/6.0f);
IMPLEMENT_DICE_ROLL_STAT_MODIFIER(CITIES, wxT("3"), "3", R3, RuleNothing, RedDie, 20, 1.0f/6.0f);
IMPLEMENT_DICE_ROLL_STAT_MODIFIER(CITIES, wxT("4"), "4", R4, RuleNothing, RedDie, 30, 1.0f/6.0f);
IMPLEMENT_DICE_ROLL_STAT_MODIFIER(CITIES, wxT("5"), "5", R5, RuleNothing, RedDie, 40, 1.0f/6.0f);
IMPLEMENT_DICE_ROLL_STAT_MODIFIER(CITIES, wxT("6"), "6", R6, RuleNothing, RedDie, 50, 1.0f/6.0f);

IMPLEMENT_DICE_ROLL_STAT_MODIFIER(CITIES, _("Barbarian"), "Barbarian", Barbarian, RuleNothing, ThirdDie, 0, 3.0f/6.0f);
IMPLEMENT_DICE_ROLL_STAT_MODIFIER(CITIES, _("Trade"), "Trade", Trade, RuleNothing, ThirdDie, 10, 1.0f/6.0f);
IMPLEMENT_DICE_ROLL_STAT_MODIFIER(CITIES, _("Politics"), "Politics", Politics, RuleNothing, ThirdDie, 20, 1.0f/6.0f);
IMPLEMENT_DICE_ROLL_STAT_MODIFIER(CITIES, _("Science"), "Science", Science, RuleNothing, ThirdDie, 30, 1.0f/6.0f);

IMPLEMENT_STAT_MODIFIER(CITIES, _("Defender Points"), "Defender Points", DefenderPoints, RuleDefenderPoint, Points, 70);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Metropolis"), "Metropolis", Metropolis, RuleSelectMetropolisCity, Points, 80);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Merchant"), "Merchant", Merchant, RuleExecuteMerchantPlacement, Points, 90);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Victory Points"), "Victory Points", ProgressCard, RuleProgressCardCommon, Points, 100);

IMPLEMENT_STAT_MODIFIER(CITIES, _("Cloth"), "Cloth", Cloth, RuleNothing, ResourcesGained, 50);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Coin"), "Coin", Coin, RuleNothing, ResourcesGained, 60);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Paper"), "Paper", Paper, RuleNothing, ResourcesGained, 70);

IMPLEMENT_STAT_MODIFIER(CITIES, _("Cloth"), "Cloth", Cloth, RuleNothing, ResourcesLost, 50);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Coin"), "Coin", Coin, RuleNothing, ResourcesLost, 60);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Paper"), "Paper", Paper, RuleNothing, ResourcesLost, 70);

IMPLEMENT_STAT_MODIFIER(CITIES, _("Cloth"), "Cloth", Cloth, RuleNothing, ResourcesBlocked, 50);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Coin"), "Coin", Coin, RuleNothing, ResourcesBlocked, 60);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Paper"), "Paper", Paper, RuleNothing, ResourcesBlocked, 70);

IMPLEMENT_STAT_MODIFIER(CITIES, _("Aqueduct"), "Aqueduct", Aqueduct, RuleExecuteAqueduct, ResourcesGainedFrom, 40);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Commercial Harbor"), "Commercial Harbor", CommercialHarbor, RuleExecuteCommercialHarbor, ResourcesGainedFrom, 50);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Master Merchant"), "Master Merchant", MasterMerchant, RuleExecuteMasterMerchant, ResourcesGainedFrom, 60);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Resource Monopoly"), "Resource Monopoly", ResourceMonopoly, RuleExecuteResourceMonopolyCard, ResourcesGainedFrom, 70);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Trade Monopoly"), "Trade Monopoly", TradeMonopoly, RuleExecuteTradeMonopolyCard, ResourcesGainedFrom, 80);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Wedding"), "Wedding", Wedding, RuleExecuteWedding, ResourcesGainedFrom, 90);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Irrigation"), "Irrigation", Irrigation, RulePlayCardIrrigation, ResourcesGainedFrom, 100);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Mining"), "Mining", Mining, RulePlayCardMining, ResourcesGainedFrom, 110);

IMPLEMENT_STAT_MODIFIER(CITIES, _("Commercial Harbor"), "Commercial Harbor", CommercialHarborLoss, RuleExecuteCommercialHarbor, ResourcesLostTo, 50);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Master Merchant"), "Master Merchant", MasterMerchantLoss, RuleExecuteMasterMerchant, ResourcesLostTo, 60);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Resource Monopoly"), "Resource Monopoly", ResourceMonopolyLoss, RuleExecuteResourceMonopolyCard, ResourcesLostTo, 70);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Trade Monopoly"), "Trade Monopoly", TradeMonopolyLoss, RuleExecuteTradeMonopolyCard, ResourcesLostTo, 80);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Arsonist"), "Arsonist", ArsonistLoss, RuleExecuteArsonist, ResourcesLostTo, 90);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Wedding"), "Wedding", WeddingLoss, RuleExecuteWedding, ResourcesLostTo, 100);

IMPLEMENT_STAT_MODIFIER(CITIES, _("Walls"), "Walls", WallsPurchased, RuleNothing, ItemsPurchased, 30);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Basic Knights"), "Basic Knights", Knight1Purchased, RuleNothing, ItemsPurchased, 40);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Strong Knights"), "Strong Knights", Knight2Purchased, RuleNothing, ItemsPurchased, 50);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Mighty Knights"), "Mighty Knights", Knight3Purchased, RuleNothing, ItemsPurchased, 60);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Knight Activates"), "Knight Activates", ActivatePurchased, RuleNothing, ItemsPurchased, 70);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Trade Upgrades"), "Trade Upgrades", TradePurchased, RuleNothing, ItemsPurchased, 80);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Politics Upgrades"), "Politics Upgrades", PoliticsPurchased, RuleNothing, ItemsPurchased, 90);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Science Upgrades"), "Science Upgrades", SciencePurchased, RuleNothing, ItemsPurchased, 100);
IMPLEMENT_STAT_MODIFIER(CITIES, _("City Repairs"), "City Repairs", CityRepairPurchased, RuleNothing, ItemsPurchased, 110);

IMPLEMENT_STAT_MODIFIER(CITIES, _("Knights Moved"), "Knights Moved", MoveKnightAction, RuleNothing, ActionsTaken, 10);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Criminals Expelled"), "Criminals Expelled", ExpelCriminalsAction, RuleNothing, ActionsTaken, 20);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Knights Attacked"), "Knights Attacked", AttackKnightAction, RuleNothing, ActionsTaken, 30);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Coommercial Harbor Used"), "Coommercial Harbor Used", CommercialHarborAction, RuleNothing, ActionsTaken, 40);

IMPLEMENT_STAT_MODIFIER(CITIES, _("Commercial Harbor"), "Commercial Harbor", CommercialHarborPlayedCard, RuleNothing, TradeCardsPlayed, 0);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Master Merchant"), "Master Merchant", MasterMerchantPlayedCard, RuleNothing, TradeCardsPlayed, 10);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Merchant"), "Merchant", MerchantPlayedCard, RuleNothing, TradeCardsPlayed, 20);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Merchant Fleet"), "Merchant Fleet", MerchantFleetPlayedCard, RuleNothing, TradeCardsPlayed, 30);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Resource Monopoly"), "Resource Monopoly", ResourceMonopolyPlayedCard, RuleNothing, TradeCardsPlayed, 40);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Trade Monopoly"), "Trade Monopoly", TradeMonopolyPlayedCard, RuleNothing, TradeCardsPlayed, 50);

IMPLEMENT_STAT_MODIFIER(CITIES, _("Arsonist"), "Arsonist", ArsonistPlayedCard, RuleNothing, PoliticsCardsPlayed, 0);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Bishop"), "Bishop", BishopPlayedCard, RuleNothing, PoliticsCardsPlayed, 10);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Constitution"), "Constitution", ConstitutionPlayedCard, RuleNothing, PoliticsCardsPlayed, 20);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Deserter"), "Deserter", DeserterPlayedCard, RuleNothing, PoliticsCardsPlayed, 30);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Diplomat"), "Diplomat", DiplomatPlayedCard, RuleNothing, PoliticsCardsPlayed, 40);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Intrigue"), "Intrigue", IntriguePlayedCard, RuleNothing, PoliticsCardsPlayed, 50);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Saboteur"), "Saboteur", SaboteurPlayedCard, RuleNothing, PoliticsCardsPlayed, 60);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Spy"), "Spy", SpyPlayedCard, RuleNothing, PoliticsCardsPlayed, 70);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Warlord"), "Warlord", WarlordPlayedCard, RuleNothing, PoliticsCardsPlayed, 80);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Wedding"), "Wedding", WeddingPlayedCard, RuleNothing, PoliticsCardsPlayed, 90);

IMPLEMENT_STAT_MODIFIER(CITIES, _("Alchemist"), "Alchemist", AlchemistPlayedCard, RuleNothing, ScienceCardsPlayed, 0);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Crane"), "Crane", CranePlayedCard, RuleNothing, ScienceCardsPlayed, 10);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Engineer"), "Engineer", EngineerPlayedCard, RuleNothing, ScienceCardsPlayed, 20);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Inventor"), "Inventor", InventorPlayedCard, RuleNothing, ScienceCardsPlayed, 30);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Irrigation"), "Irrigation", IrrigationPlayedCard, RuleNothing, ScienceCardsPlayed, 40);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Medicine"), "Medicine", MedicinePlayedCard, RuleNothing, ScienceCardsPlayed, 50);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Mining"), "Mining", MiningPlayedCard, RuleNothing, ScienceCardsPlayed, 60);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Printer"), "Printer", PrinterPlayedCard, RuleNothing, ScienceCardsPlayed, 70);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Road Building"), "Road Building", RoadBuildingPlayedCard, RuleNothing, ScienceCardsPlayed, 80);
IMPLEMENT_STAT_MODIFIER(CITIES, _("Smith"), "Smith", SmithPlayedCard, RuleNothing, ScienceCardsPlayed, 90);


//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
