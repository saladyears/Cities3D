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
#include "Map.h"
#include "MerchantObject.h"
#include "AggregatorObject.h"
#include "Controller.h"
#include "GameEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- RULES            -----------------------------//

//-----------------------------------------------------------------------//
// Class: RuleInitializeCities
// 
// Initializes data for the <Game> and <PlayerGames> at the
// start of the game, based on the requirements of the <Cities>
// <RuleSet>.
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
// Mixin To:
//     <RuleInitializeData>
//
class RuleInitializeCities : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// Cities games always start with 3 more points.
		gameData<wxInt32>(shPoints) += 3;

		//initialize game data needed
		gameData<wxInt32>(shBarbarianAdvance) = 0;
		gameData<wxInt32>(shAllowRobberPlace) = FALSE;
		gameData<wxInt32>(shTradePlayer) = -1;
		gameData<wxInt32>(shPoliticsPlayer) = -1;
		gameData<wxInt32>(shSciencePlayer) = -1;
		gameData<wxInt32>(shMerchantPlayer) = -1;
		gameData<wxInt32>(shAlchemistDie1) = 0;
		gameData<wxInt32>(shAlchemistDie2) = 0;

		//is this a 3-4 player map?
		bool b34 = (5 > MAP.read<wxInt32>(shMinPlayers));

		wxInt32 bankRes = b34 ? 19 : 24;

		//set bank commodities properly
		gameData<Data::IntHash>(shBankResources)[shPaper] = bankRes;
		gameData<Data::IntHash>(shBankResources)[shCloth] = bankRes;
		gameData<Data::IntHash>(shBankResources)[shCoin] = bankRes;

		gameData<Data::IntHash>(shOriginalBankResources)[shPaper] = bankRes;
		gameData<Data::IntHash>(shOriginalBankResources)[shCloth] = bankRes;
		gameData<Data::IntHash>(shOriginalBankResources)[shCoin] = bankRes;

		//set card quantities
		gameData<Data::IntHash>(shBankScienceCards)[shAlchemist] = 2;
		gameData<Data::IntHash>(shBankScienceCards)[shCrane] = 2;
		gameData<Data::IntHash>(shBankScienceCards)[shEngineer] = 2;
		gameData<Data::IntHash>(shBankScienceCards)[shInventor] = 1;
		gameData<Data::IntHash>(shBankScienceCards)[shIrrigation] = 2;
		gameData<Data::IntHash>(shBankScienceCards)[shMedicine] = 2;
		gameData<Data::IntHash>(shBankScienceCards)[shMining] = 2;
		gameData<Data::IntHash>(shBankScienceCards)[shPrinter] = 1;
		gameData<Data::IntHash>(shBankScienceCards)[shRoadBuilding] = 2;
		gameData<Data::IntHash>(shBankScienceCards)[shSmith] = 2;

		gameData<Data::IntHash>(shBankPoliticsCards)[shArsonist] = 2;
		gameData<Data::IntHash>(shBankPoliticsCards)[shBishop] = 2;
		gameData<Data::IntHash>(shBankPoliticsCards)[shConstitution] = 1;
		gameData<Data::IntHash>(shBankPoliticsCards)[shDeserter] = 2;
		gameData<Data::IntHash>(shBankPoliticsCards)[shDiplomat] = 2;
		gameData<Data::IntHash>(shBankPoliticsCards)[shIntrigue] = 2;
		gameData<Data::IntHash>(shBankPoliticsCards)[shSaboteur] = 0;
		gameData<Data::IntHash>(shBankPoliticsCards)[shSpy] = 3;
		gameData<Data::IntHash>(shBankPoliticsCards)[shWarlord] = 2;
		gameData<Data::IntHash>(shBankPoliticsCards)[shWedding] = 2;

		gameData<Data::IntHash>(shBankTradeCards)[shCommercialHarbor] = 2;
		gameData<Data::IntHash>(shBankTradeCards)[shMasterMerchant] = 2;
		gameData<Data::IntHash>(shBankTradeCards)[shMerchant] = 6;
		gameData<Data::IntHash>(shBankTradeCards)[shMerchantFleet] = 2;
		gameData<Data::IntHash>(shBankTradeCards)[shResourceMonopoly] = 4;
		gameData<Data::IntHash>(shBankTradeCards)[shTradeMonopoly] = 2;

		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			playerGameData<wxInt32>(shDefenderPoints, i) = 0;

			playerGameData<wxInt32>(shStockWalls, i) = 3;
			playerGameData<wxInt32>(shStockKnights1, i) = 2;
			playerGameData<wxInt32>(shStockKnights2, i) = 2;
			playerGameData<wxInt32>(shStockKnights3, i) = 2;
			playerGameData<wxInt32>(shLoseCity, i) = 0;
			playerGameData<wxInt32>(shWedding, i) = FALSE;
			playerGameData<wxInt32>(shArsonist, i) = FALSE;
			playerGameData<wxInt32>(shAlchemist, i) = FALSE;
			playerGameData<wxInt32>(shGainProgressCard, i) = 0;
			playerGameData<wxInt32>(shCommercialHarbor, i) = FALSE;
			playerGameData<wxInt32>(shFreeKnightUpgrades, i) = 0;
			playerGameData<wxInt32>(shFreeWalls, i) = 0;
			playerGameData<wxInt32>(shFreeCityUpgrades, i) = 0;

			playerGameData<wxInt32>(shTradeLevel, i) = 0;
			playerGameData<wxInt32>(shPoliticsLevel, i) = 0;
			playerGameData<wxInt32>(shScienceLevel, i) = 0;

			playerGameData<IntArray>(shSabotagedCities, i);

			playerGameData<HashStringArray>(shMerchantFleetCards, i);

			//commodities
			playerGameData<Data::IntHash>(shResources, i)[shPaper] = 0;
			playerGameData<Data::IntHash>(shResources, i)[shCloth] = 0;
			playerGameData<Data::IntHash>(shResources, i)[shCoin] = 0;

			//science cards
			playerGameData<Data::IntHash>(
				shPlayableScienceCards, i)[shAlchemist] = 0;
			playerGameData<Data::IntHash>(
				shPlayableScienceCards, i)[shCrane] = 0;
			playerGameData<Data::IntHash>(
				shPlayableScienceCards, i)[shEngineer] = 0;
			playerGameData<Data::IntHash>(
				shPlayableScienceCards, i)[shInventor] = 0;
			playerGameData<Data::IntHash>(
				shPlayableScienceCards, i)[shIrrigation] = 0;
			playerGameData<Data::IntHash>(
				shPlayableScienceCards, i)[shMedicine] = 0;
			playerGameData<Data::IntHash>(
				shPlayableScienceCards, i)[shMining] = 0;
			playerGameData<Data::IntHash>(
				shPlayableScienceCards, i)[shPrinter] = 0;
			playerGameData<Data::IntHash>(
				shPlayableScienceCards, i)[shRoadBuilding] = 0;
			playerGameData<Data::IntHash>(
				shPlayableScienceCards, i)[shSmith] = 0;

			playerGameData<Data::IntHash>(
				shPlayedScienceCards, i)[shAlchemist] = 0;
			playerGameData<Data::IntHash>(
				shPlayedScienceCards, i)[shCrane] = 0;
			playerGameData<Data::IntHash>(
				shPlayedScienceCards, i)[shEngineer] = 0;
			playerGameData<Data::IntHash>(
				shPlayedScienceCards, i)[shInventor] = 0;
			playerGameData<Data::IntHash>(
				shPlayedScienceCards, i)[shIrrigation] = 0;
			playerGameData<Data::IntHash>(
				shPlayedScienceCards, i)[shMedicine] = 0;
			playerGameData<Data::IntHash>(
				shPlayedScienceCards, i)[shMining] = 0;
			playerGameData<Data::IntHash>(
				shPlayedScienceCards, i)[shPrinter] = 0;
			playerGameData<Data::IntHash>(
				shPlayedScienceCards, i)[shRoadBuilding] = 0;
			playerGameData<Data::IntHash>(
				shPlayedScienceCards, i)[shSmith] = 0;

			//politics cards
			playerGameData<Data::IntHash>(
				shPlayablePoliticsCards, i)[shArsonist] = 0;
			playerGameData<Data::IntHash>(
				shPlayablePoliticsCards, i)[shBishop] = 0;
			playerGameData<Data::IntHash>(
				shPlayablePoliticsCards, i)[shConstitution] = 0;
			playerGameData<Data::IntHash>(
				shPlayablePoliticsCards, i)[shDeserter] = 0;
			playerGameData<Data::IntHash>(
				shPlayablePoliticsCards, i)[shDiplomat] = 0;
			playerGameData<Data::IntHash>(
				shPlayablePoliticsCards, i)[shIntrigue] = 0;
			playerGameData<Data::IntHash>(
				shPlayablePoliticsCards, i)[shSaboteur] = 0;
			playerGameData<Data::IntHash>(
				shPlayablePoliticsCards, i)[shSpy] = 0;
			playerGameData<Data::IntHash>(
				shPlayablePoliticsCards, i)[shWarlord] = 0;
			playerGameData<Data::IntHash>(
				shPlayablePoliticsCards, i)[shWedding] = 0;

			playerGameData<Data::IntHash>(
				shPlayedPoliticsCards, i)[shArsonist] = 0;
			playerGameData<Data::IntHash>(
				shPlayedPoliticsCards, i)[shBishop] = 0;
			playerGameData<Data::IntHash>(
				shPlayedPoliticsCards, i)[shConstitution] = 0;
			playerGameData<Data::IntHash>(
				shPlayedPoliticsCards, i)[shDeserter] = 0;
			playerGameData<Data::IntHash>(
				shPlayedPoliticsCards, i)[shDiplomat] = 0;
			playerGameData<Data::IntHash>(
				shPlayedPoliticsCards, i)[shIntrigue] = 0;
			playerGameData<Data::IntHash>(
				shPlayedPoliticsCards, i)[shSaboteur] = 0;
			playerGameData<Data::IntHash>(
				shPlayedPoliticsCards, i)[shSpy] = 0;
			playerGameData<Data::IntHash>(
				shPlayedPoliticsCards, i)[shWarlord] = 0;
			playerGameData<Data::IntHash>(
				shPlayedPoliticsCards, i)[shWedding] = 0;

			//trade cards
			playerGameData<Data::IntHash>(
				shPlayableTradeCards, i)[shCommercialHarbor] = 0;
			playerGameData<Data::IntHash>(
				shPlayableTradeCards, i)[shMasterMerchant] = 0;
			playerGameData<Data::IntHash>(
				shPlayableTradeCards, i)[shMerchant] = 0;
			playerGameData<Data::IntHash>(
				shPlayableTradeCards, i)[shMerchantFleet] = 0;
			playerGameData<Data::IntHash>(
				shPlayableTradeCards, i)[shResourceMonopoly] = 0;
			playerGameData<Data::IntHash>(
				shPlayableTradeCards, i)[shTradeMonopoly] = 0;

			playerGameData<Data::IntHash>(
				shPlayedTradeCards, i)[shCommercialHarbor] = 0;
			playerGameData<Data::IntHash>(
				shPlayedTradeCards, i)[shMasterMerchant] = 0;
			playerGameData<Data::IntHash>(
				shPlayedTradeCards, i)[shMerchant] = 0;
			playerGameData<Data::IntHash>(
				shPlayedTradeCards, i)[shMerchantFleet] = 0;
			playerGameData<Data::IntHash>(
				shPlayedTradeCards, i)[shResourceMonopoly] = 0;
			playerGameData<Data::IntHash>(
				shPlayedTradeCards, i)[shTradeMonopoly] = 0;
		}

		//the game should always have a robber (start it off not visible)
		HexObjectPtr merchant(new MerchantObject(0));
		merchant->set(IGameObject::Hidden);
		GetGame()->setHexObject(shMerchant, merchant);

		//send it off to the view
		AggregatorObjectPtr aggregate(new AggregatorObject);
		aggregate->add(merchant);
		Controller::get().Transmit(shEventGameObjects, aggregate);
	}
};

IMPLEMENT_RULE_MIXIN(RuleInitializeCities, RuleInitializeData, CITIES)
