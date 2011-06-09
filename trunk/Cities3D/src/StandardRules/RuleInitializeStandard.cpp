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
#include "GameEngine.h"
#include "RobberObject.h"
#include "AggregatorObject.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleInitializeStandard
// 
// Initializes data for the <Game> and <PlayerGames> at the
// start of the game, based on the requirements of the <Standard>
// <RuleSet>.
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
// Mixin To:
//     <RuleInitializeData>
//
// Transmits To:
//     eventGameObjects
//
class RuleInitializeStandard : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
        //initialize game data needed
		gameData<wxInt32>(shCurrentPlayer) = 0;
		gameData<wxInt32>(shLongestRoadPlayer) = -1;
		gameData<wxInt32>(shLargestArmyPlayer) = -1;
		gameData<wxInt32>(shAutoTradeComplete) = FALSE;
		gameData<wxInt32>(shTurn) = 0;

		gameData<HashString>(shAutoTradeLogic) = shEmpty;
		gameData<HashString>(shAutoTradeRule) = shEmpty;

		//initialize player data
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			playerGameData<wxInt32>(shStockRoads, i) = 
				MAP.read<wxInt32>(shStockRoads);
			playerGameData<wxInt32>(shStockSettlements, i) = 
				MAP.read<wxInt32>(shStockSettlements);
			playerGameData<wxInt32>(shStockCities, i) = 
				MAP.read<wxInt32>(shStockCities);
			playerGameData<wxInt32>(shRoadLength, i) = 0;
			playerGameData<wxInt32>(shAllowedCards, i) = 
				MAP.read<wxInt32>(shAllowedCards);

			playerGameData<wxInt32>(shPlayedCard, i) = 0;
			playerGameData<wxInt32>(shFreeRoads, i) = 0;
			playerGameData<wxInt32>(shFreeCities, i) = 0;
			playerGameData<wxInt32>(shLoseCards, i) = 0;
            playerGameData<wxInt32>(shResourcesGained, i) = 0;
			
			playerGameData<Data::IntHash>(
				shResources, i)[shTimber] = 0;
			playerGameData<Data::IntHash>(
				shResources, i)[shClay] = 0;
			playerGameData<Data::IntHash>(
				shResources, i)[shSheep] = 0;
			playerGameData<Data::IntHash>(
				shResources, i)[shWheat] = 0;
			playerGameData<Data::IntHash>(
				shResources, i)[shOre] = 0;
		}

		//is this a 3-4 player map?
		bool b34 = (5 > MAP.read<wxInt32>(shMinPlayers));

		wxInt32 bankRes = b34 ? 19 : 24;

		// Set bank resources properly.
		gameData<Data::IntHash>(shBankResources)[shTimber] = bankRes;
		gameData<Data::IntHash>(shBankResources)[shClay] = bankRes;
		gameData<Data::IntHash>(shBankResources)[shSheep] = bankRes;
		gameData<Data::IntHash>(shBankResources)[shWheat] = bankRes;
		gameData<Data::IntHash>(shBankResources)[shOre] = bankRes;

		gameData<Data::IntHash>(shOriginalBankResources)[shTimber] = bankRes;
		gameData<Data::IntHash>(shOriginalBankResources)[shClay] = bankRes;
		gameData<Data::IntHash>(shOriginalBankResources)[shSheep] = bankRes;
		gameData<Data::IntHash>(shOriginalBankResources)[shWheat] = bankRes;
		gameData<Data::IntHash>(shOriginalBankResources)[shOre] = bankRes;

		//the game should always have a robber (start it off not visible)
		HexObjectPtr robber(new RobberObject(0));
		robber->set(IGameObject::Hidden);
		GetGame()->setHexObject(shRobber, robber);

		//send it off to the view
		AggregatorObjectPtr aggregate(new AggregatorObject);
		aggregate->add(robber);
		Controller::get().Transmit(shEventGameObjects, aggregate);
	}
};

IMPLEMENT_RULE_MIXIN(RuleInitializeStandard, RuleInitializeData, STANDARD);
