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
#include "KnightObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleResetBarbarians
// 
// Resets the barbarians, deactivates all knights, and continues with roll
// resolution.
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
//
class RuleResetBarbarians : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// All knights become inactive.
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			const PlayerGame::CornerObjectArray& knights =
				playerGame(i).getCornerObjects(shKnights);

			PlayerGame::CornerObjectArray::const_iterator it,
				itEnd = knights.end();
			for(it = knights.begin(); it != itEnd; ++it)
			{
				KnightObject* knight = static_cast<KnightObject*>(it->get());
				knight->active(false, -1);
			}
		}

		// Barbarians go back to their origin.
		gameData<wxInt32>(shBarbarianAdvance) = 0;
		Controller::get().Transmit(shEventKnightCounts, GetGame());
		Controller::get().Transmit(shEventBarbarianAdvance, GetGame());
		
		RULE.Execute(shRuleMarkerPostDiceRoll, DataObject());
	}
};

IMPLEMENT_RULE(RuleResetBarbarians, CITIES)
