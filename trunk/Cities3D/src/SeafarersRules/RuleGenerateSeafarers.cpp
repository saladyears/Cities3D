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
#include "GameEngine.h"
#include "RuleSetDatabase.h"
#include "Controller.h"
#include "AggregatorObject.h"
#include "PortObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleGenerateSeafarers
// 
// Generates <PortObjects> for port <TileObjects>.  Sends them
// up the to View for display via the eventGameObjects <Event>.
//
// Derived From:
//     <Rule>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <Seafarers>
//
// Mixin To:
//     <RuleGenerateBoard>
//
// Logic Called:
//     <LogicTileIsPort>
//
// Transmits To:
//     eventGameObjects
//
class RuleGenerateSeafarers : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		AggregatorObjectPtr aggregate(new AggregatorObject);

		//update all of the tiles to have port objects
		for(wxInt32 i = 0; i < numTiles(); ++i)
		{
			DataObject input(i), output;
			RULE.Decide(shLogicTileIsPort, input, output);

			if(true == output.read<bool>())
			{
				tile(i)->createPortObject(true);
				aggregate->add(tile(i)->port());
			}
		}

		//send it to the view
		Controller::get().Transmit(shEventGameObjects, aggregate);
	}
};

IMPLEMENT_RULE_MIXIN(RuleGenerateSeafarers, RuleGenerateBoard, SEAFARERS);
