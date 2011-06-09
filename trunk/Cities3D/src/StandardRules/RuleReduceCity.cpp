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
#include "Controller.h"
#include "Utility.h"
#include "CornerObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleReduceCity
// 
// Reduces the given city to a settlement.
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
//
class RuleReduceCity : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		// Clear the board.
		RULE.Execute(shRuleResetTileFlags, DataObject());
		RULE.Execute(shRuleResetObjectFlags, DataObject());

		// Find the city that's being destroyed and get its tile and corner.
		const PlayerGame::CornerObjectArray& cities = 
			playerGame().getCornerObjects(shCities);

		wxInt32 id = object.read<wxInt32>();
		CornerObjectPtr city;

		PlayerGame::CornerObjectArray::const_iterator it, 
			itEnd = cities.end();
		for(it = cities.begin(); it != itEnd; ++it)
		{
			if((*it)->id() == id)
			{
				city = (*it);
				break;
			}
		}
		wxASSERT(city);

		wxInt32 tile = city->tile1().first;
		wxInt32 corner = city->tile1().second;

		DataObject thisObject(Utility::encodeSel(tile, corner), current());

		RULE.Execute(shRuleReduceCityCommon, thisObject);
		
		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), current()));

		// Update the UI.
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stHasACityReduced));
	}
};

IMPLEMENT_RULE(RuleReduceCity, STANDARD)
