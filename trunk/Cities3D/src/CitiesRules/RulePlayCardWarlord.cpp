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
#include "KnightObject.h"
#include "IRuleEngine.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePlayCardWarlord
// 
// Plays a warlord card.
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
class RulePlayCardWarlord : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		const HashString& card = object.read<HashString>();
		
		RULE.Execute(shRulePlayProgressCardStart, DataObject(card, shPolitics));

		wxInt32 turn = gameData<wxInt32>(shTurn);

		// The Warlord activates all knights the player owns for free.
		const PlayerGame::CornerObjectArray& knights =
			playerGame().getCornerObjects(shKnights);
		PlayerGame::CornerObjectArray::const_iterator it, 
			itEnd = knights.end();
		for(it = knights.begin(); it != itEnd; ++it)
		{
			KnightObject* knight = 
				static_cast<KnightObject*>(it->get());
			wxASSERT(NULL != knight);
			knight->active(true, turn);
		}

		// If the player activated any knights, we need to check knight counts
		// again.
		if(false == knights.empty())
		{
			Controller::get().Transmit(shEventKnightCounts, GetGame());
			Controller::get().Transmit(shEventDirty, 0);
		}

		// Play the sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_PLAY_WARLORD_CARD));

		RULE.Execute(shRulePlayProgressCardEnd, DataObject());
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RulePlayCardWarlord, Warlord, 
						   RulePlayCard, CITIES)
