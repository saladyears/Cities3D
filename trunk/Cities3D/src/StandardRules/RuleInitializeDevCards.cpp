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

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleInitializeDevCards
// 
// Initializes data for the <Game> and <PlayerGames> at the
// start of the game, based on the requirements of the <DevCards>
// <RuleSet>.
//
// Derived From:
//     <Rule>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <DevCards>
//
// Mixin To:
//     <RuleInitializeData>
//
class RuleInitializeDevCards : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		//is this a 3-4 player map?
		bool b34 = (5 > MAP.read<wxInt32>(shMinPlayers));

		//some roads could be free now
		gameData<wxInt32>(shFreeRoads) = 0;

		//initialize game data needed
		gameData<Data::IntHash>(shDevCards)[shDev] = b34 ? 25 : 34;
		gameData<Data::IntHash>(shOriginalDevCards)[shDev] = b34 ? 25 : 34;

		//set bank development card amounts
		gameData<Data::IntHash>(shBankDevCards)[shVictoryPoint] = 5;
		gameData<Data::IntHash>(shBankDevCards)[shRoadBuilding] = 
			b34 ? 2 : 3;
		gameData<Data::IntHash>(shBankDevCards)[shYearOfPlenty] =
			b34 ? 2 : 3;
		gameData<Data::IntHash>(shBankDevCards)[shMonopoly] = 
			b34 ? 2 : 3;
		gameData<Data::IntHash>(shBankDevCards)[shSoldier] =
			b34 ? 14 : 20;

		//initialize player data
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			playerGameData<Data::IntHash>(
				shPurchasedCards, i)[shRoadBuilding] = 0;
			playerGameData<Data::IntHash>(
				shPurchasedCards, i)[shYearOfPlenty] = 0;
			playerGameData<Data::IntHash>(
				shPurchasedCards, i)[shMonopoly] = 0;
			playerGameData<Data::IntHash>(
				shPurchasedCards, i)[shSoldier] = 0;

			playerGameData<Data::IntHash>(
				shPlayableCards, i)[shVictoryPoint] = 0;
			playerGameData<Data::IntHash>(
				shPlayableCards, i)[shRoadBuilding] = 0;
			playerGameData<Data::IntHash>(
				shPlayableCards, i)[shYearOfPlenty] = 0;
			playerGameData<Data::IntHash>(
				shPlayableCards, i)[shMonopoly] = 0;
			playerGameData<Data::IntHash>(
				shPlayableCards, i)[shSoldier] = 0; 

			playerGameData<Data::IntHash>(
				shPlayedCards, i)[shVictoryPoint] = 0;
			playerGameData<Data::IntHash>(
				shPlayedCards, i)[shRoadBuilding] = 0;
			playerGameData<Data::IntHash>(
				shPlayedCards, i)[shYearOfPlenty] = 0;
			playerGameData<Data::IntHash>(
				shPlayedCards, i)[shMonopoly] = 0;
			playerGameData<Data::IntHash>(
				shPlayedCards, i)[shSoldier] = 0;
		}
	}
};

IMPLEMENT_RULE_MIXIN(RuleInitializeDevCards, RuleInitializeData, DEVCARDS)
