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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleSelectDiplomatRoadSeafarers
// 
// Removes the selected road or ship.
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
//     <RulePlayCard>
//
class RuleSelectDiplomatRoadSeafarers : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxInt32 id = object.read<wxInt32>();

		RULE.Execute(shRuleSelectDiplomatRoadCommon, DataObject(id, shRoad, 
			stRemovesARoadOf));

		RULE.Execute(shRuleSelectDiplomatRoadCommon, DataObject(id, shShip,
			stRemovesAShipOf));
	}
};

IMPLEMENT_RULE_REPLACE(RuleSelectDiplomatRoadSeafarers, 
					   RuleSelectDiplomatRoad, CITIES)
