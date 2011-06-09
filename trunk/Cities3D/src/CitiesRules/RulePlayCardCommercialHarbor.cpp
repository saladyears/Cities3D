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
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePlayCardCommercialHarbor
// 
// Plays a commercial harbor card.
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
class RulePlayCardCommercialHarbor : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		const HashString& card = object.read<HashString>();
		
		RULE.Execute(shRulePlayProgressCardStart, DataObject(card, shTrade));

		// Set things up to be able to use the Commercial Harbor throughout
		// the rest of the turn. 
		wxInt32 curPlayer = current();
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			if(i != curPlayer)
			{
				playerGameData<wxInt32>(shCommercialHarbor, i) = TRUE;
			}
		}

		// Play the sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_PLAY_COMMERCIAL_HARBOR_CARD));

		RULE.Execute(shRulePlayProgressCardEnd, DataObject());
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RulePlayCardCommercialHarbor, CommercialHarbor, 
						   RulePlayCard, CITIES)
