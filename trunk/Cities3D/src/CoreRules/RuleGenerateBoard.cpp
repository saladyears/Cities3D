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
#include "RuleExtensionMixin.h"
#include "RuleSetDatabase.h"
#include "GameEngine.h"
#include "Controller.h"
#include "Map.h"
#include "AggregatorObject.h"
#include "ChipObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleGenerateBoard
// 
// Prepares all <Game> and <Map> data for playing.  Calls <BuildBoard>
// in <Game> to copy all <Map> data from the current map into the
// playable board.  Calls <RuleInitializeData> to allow all <RuleSets>
// to create needed data fields in the <Game> and <PlayerGame> data.
// Calls <RuleGeneratePorts> to create any needed random ports for
// the map.  Executes all extensions to RuleGenerateBoard so <RuleSets> can do
// any custom board generation they require.  Calls <RuleFindPorts>
// to store port locations in the <Game> data and orient them properly on
// the board.  Creates <ChipObjects> for all <TileObjects> in the board that
// are resource tiles.  Transmits all <TileObjects> and <ChipObjects> to
// the View for display vie the eventGameObjects <Event>.
//
// Derived From:
//     <Rule>,
//     <RuleExtensionMixin>
//
// Project:
//     <CoreRules>
//
// RuleSet:
//     <Core>
//
// Rules Called:
//     <RuleInitializeData>,
//     <RuleInitializeOptions>,
//     <RuleGeneratePorts>,
//     <RuleFindPorts>
//
// Logic Called:
//     <LogicTileIsResource>
//
// Transmits To:
//     eventGameObjects
//

class RuleGenerateBoard 
: public Rule
, public RuleExtensionMixin
{
public:
	virtual void Execute(const DataObject &object)
	{
		//generate the board tiles
		GetGame()->buildBoard(MAP.tiles_begin(), MAP.tiles_end());

        // Set the initial points.
        gameData<wxInt32>(shPoints) = MAP.read<wxInt32>(shPoints);

		RULE.Execute(shRuleInitializeData, object);
		RULE.Execute(shRuleInitializeOptions, object);
        RULE.Execute(shRuleInitializeOptionOverrides, object);
		RULE.Execute(shRuleGeneratePorts, object);

		RULE.Execute(shRulePreGenerateBoard, object);

		//call all the extensions
		ExecuteExtensions(object);

		// Allow any post processes to work.
		RULE.Execute(shRulePostGenerateBoard, object);

		//set all ports and rotations properly
		RULE.Execute(shRuleFindPorts, object);

		//create a new aggregator so that all of these tiles can be sent to the
		//display engine
		AggregatorObjectPtr aggregate(new AggregatorObject);

		//add all the tiles and create chip objects where needed
		for(wxInt32 i = 0; i < numTiles(); ++i)
		{
			//add all the tiles to the aggregator
			aggregate->add(tile(i));

			DataObject input(i), output;
			RULE.Decide(shLogicTileIsResource, input, output);

			if(true == output.read<bool>())
			{
				tile(i)->createChipObject(true);
				tile(i)->chip()->randomizeRotation();
				aggregate->add(tile(i)->chip());
			}
		}

		//notify the display engine
		Controller::get().Transmit(shEventGameObjects, aggregate);
	}
};

IMPLEMENT_RULE(RuleGenerateBoard, CORE);
