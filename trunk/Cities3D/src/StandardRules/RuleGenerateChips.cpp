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
#include "GameEngine.h"
#include "Map.h"
#include "RNG.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	// constant chip lookup values
	const wxInt32 s34Chips[] = {5, 2, 6, 3, 8, 10, 9, 12, 11, 4, 8, 10, 9, 4,\
		5, 6, 3, 11};
	const wxInt32 s56Chips[] = {2, 5, 4, 6, 3, 9, 8, 11, 11, 10, 6, 3, 8, 4,\
		8, 10, 11, 12, 10, 5, 4, 9, 5, 9, 12, 3, 2, 6};
}

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleGenerateChips
// 
// Generates an around-the-board chip layout using the chip
// sequences from the Standard Settlers 3-4 player and 5-6
// player boards.
//
// Derived From:
//     <Rule>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <AroundTheBoard>
//
// Mixin To:
//     <RuleGenerateBoard>
//
// Rules Called:
//     <RulePlaceRobber>
//
// Logic Called:
//     <LogicTileIsOcean>
//
class RuleGenerateChips : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		//this function lays out the chips for an around-the-board style of 
		//laying them out, as used in the Standard and New Shores official maps;
		//the algorithm picks a random corner and direction to go from and lays
		//the chip values circling inwards until there are no longer any valid 
		//values to lay another chip on
		
		//determine which corner to start on
		wxInt32 corner = RAND.pooled(6);

		//get the initial tile
		wxInt32 thisTile = MAP.read<IntArray>(shRollPlaces)[corner];

		//determine clockwise or non clockwise
		bool bClockwise = RAND.pooled(2) ? true : false;

		//create the array of values used by the algorithm
		std::vector<bool> bTiles(numTiles(), false);
		
		//see which type of chipset we need to use
		bool b34 = (3 == MAP.read<wxInt32>(shMinPlayers)) ||
				   (4 == MAP.read<wxInt32>(shMinPlayers));

		//get the list of lookup values
		const wxInt32 *pRoll = b34 ? &s34Chips[0] : &s56Chips[0];

		//continue looping through the values while we can
		wxInt32 chip = 0;
		wxInt32 nextTile = 0;
		bool bContinue = true;
		while(true == bContinue)
		{
			//set the current tile
			if(shDesert != tile<HashString>(shTileType, thisTile))
			{
				tile<wxInt32>(shRoll, thisTile) = pRoll[chip];

				//increment the chip counter
				chip++;
			}
			//otherwise, set the robber on the desert
			else
			{
				DataObject input(0), output;
				RULE.Decide(shLogicCanPlaceRobber, input, output);
				
				if(true == output.read<bool>())
				{
					RULE.Execute(shRulePlaceRobber, DataObject(thisTile));
				}
			}

			//set this tile as used
			bTiles[thisTile] = true;
			
			//if we have reached a tile that is an ocean tile or already used,
			//we must adjust the corner
			bool bValid = false;
			for(wxInt32 i = 0; i < 6; i++)
			{
				//get the next tile
				if(true == bClockwise)
				{
					nextTile = tile<IntArray>(
						shSides, thisTile)[(corner + 4) % 6];
				}
				else
				{
					nextTile = tile<IntArray>(
						shSides, thisTile)[(corner + 2) % 6];
				}

				DataObject input(nextTile), output;
				RULE.Decide(shLogicTileIsOcean, input, output);
				
				if( (true == output.read<bool>()) ||
					(true == bTiles[nextTile]))
				{
					//set the new corner and try it again
					if(true == bClockwise)
					{
						//subtract one
						corner = (corner + 5) % 6;
					}
					else
					{
						//add one
						corner = (corner + 1) % 6;
					}
				}
				else
				{
					bValid = true;
					break;
				}
			}

			//see if we're done
			if(false == bValid)
			{
				bContinue = false;
				continue;
			}

			//set the new tile
			thisTile = nextTile;
		}
	}
};

IMPLEMENT_RULE_MIXIN(RuleGenerateChips, RuleGenerateBoard, AROUND_THE_BOARD);
