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
// Class: RulePlayCardWedding
// 
// Plays a bishop card.
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
class RulePlayCardWedding : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		const HashString& card = object.read<HashString>();
		
		RULE.Execute(shRulePlayProgressCardStart, DataObject(card, shPolitics));

		// Play the sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_PLAY_WEDDING_CARD));

		// Mark all players with more points than this player (and resources)
		// as having to pay for the Wedding.
		wxInt32 points = playerGameData<wxInt32>(shPoints);

		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			if(points < playerGameData<wxInt32>(shPoints, i))
			{
				// The player must also have cards to donate.
				wxInt32 total = 0;
				const Data::IntHash& resources = 
					playerGameData<Data::IntHash>(shResources, i);
				Data::IntHash::const_iterator it, itEnd = resources.end();
				for(it = resources.begin(); it != itEnd; ++it)
				{
					total += it->second;
				}

				if(0 < total)
				{
					playerGameData<wxInt32>(shWedding, i) = TRUE;
				}
			}
		}

		// Run it.
		gameData<HashString>(shState) = shTurnWedding;
		RULE.Execute(shRuleRestartTurnWedding, DataObject());
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RulePlayCardWedding, Wedding, 
						   RulePlayCard, CITIES)
