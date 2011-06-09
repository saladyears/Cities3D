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
#include "LogicHashMixin.h"
#include "RuleSetDatabase.h"
#include "GameEngine.h"
#include "UtilityGL.h"
#include "AggregatorObject.h"
#include "SideSelectionObject.h"
#include "Controller.h"
#include "Utility.h"
#include "UtilityGL.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleLocateInitialRoad
// 
// Creates a <SideSelectionObject> that contains the available
// locations for the current player to place a road after
// placing an initial settlement.  It must be given the name
// of <ILogic> that determines valid road positions in the first
// position of the given <DataObject>, and the name of the
// <IRule> that will handle the user's selection in the second
// position of the given <DataObject>.
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
// Transmits To:
//     eventSelectionObject
//
class RuleLocateInitialRoad : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		HashString logic = object.read<HashString>(1);
		HashString rule = object.read<HashString>(2);

		if( (false == logic.empty()) &&
			(false == rule.empty()))
		{
			ColorType color = playerGame().color();

			SideSelectionObject *pObject;
			pObject = new SideSelectionObject(rule, color);

			//get the tile and corner of the settlement placement out of the 
			//object
			wxInt32 thisTile, corner;
			boost::tie(thisTile, corner) = 
				Utility::decodeSel(object.read<wxInt32>());

			//examine the three road locations to see if they are valid
			testPlacement(logic, pObject, thisTile, corner);
			testPlacement(logic, pObject, thisTile, (corner + 5) % 6);

			//opposite tile and side
			wxInt32 otherTile = tile<IntArray>(shSides, thisTile)[corner];
			if(-1 != otherTile)
			{
				testPlacement(logic, pObject, otherTile, (corner + 4) % 6);
			}

			//pass the selection object along
			Controller::get().Transmit(shEventSelectionObject, 
				SelectionObjectPtr(pObject));
		}
	}

private:
	void testPlacement(const HashString &logic, SideSelectionObject *pObject, 
		const wxInt32 thisTile, const wxInt32 side)
	{
		DataObject input(std::make_pair(thisTile, side)), output;
		RULE.Decide(logic, input, output);
		
		if(true == output.read<bool>())
		{
			float rotation;
			Vector coords = tile(thisTile)->coords();
			UtilityGL::sideCoords(coords, rotation, side);
			pObject->add(SideSelectionObject::SideSelectionTuple(
				Utility::encodeSel(thisTile, side), coords, rotation));
		}
	}
};

IMPLEMENT_RULE(RuleLocateInitialRoad, STANDARD);
