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
#include "RNG.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleBarbarianAdvance
// 
// Advances the barbarians and handles the consequences.
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
// Transmits To:
//     eventBarbarianAdvance
//
class RuleBarbarianAdvance : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		wxInt32& place = gameData<wxInt32>(shBarbarianAdvance);

		// Advance the barbarians and see if they have reached land.
		++place;
		wxLogDebug(wxT("Barbarians advance to %d."), place);

		Controller::get().Transmit(shEventBarbarianAdvance, GetGame());

		if(7 <= place)
		{
			// We've hit.
			RULE.Execute(shRuleBarbarianAttack, DataObject());
		}
		else
		{
			RULE.Execute(shRulePlaySound, DataObject(SOUND_BARBARIAN_ADVANCE));

			RULE.Execute(shRuleGenerateSystemMessage, DataObject(stTheBarbariansAdvance));

			// We didn't hit, continue with roll resolution.
			RULE.Execute(shRuleMarkerPostDiceRoll, DataObject());
		}
	}
};

IMPLEMENT_RULE(RuleBarbarianAdvance, CITIES)
